#include "texture.h"

#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define MAKEFOURCC(ch0, ch1, ch2, ch3)                          \
  ((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) | \
   ((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24))

GLuint Texture::create(GLenum target, GLint internal_format, int width,
                       int height, GLenum format, GLenum type,
                       const void* image) {
  target_ = target;
  glGenTextures(1, &id_);
  glBindTexture(target_, id_);

  glTexImage2D(target_, 0, internal_format, width, height, 0, format, type,
               image);

  glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // glTexParameteri(target_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  // glTexParameteri(target_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

  return id_;
}

void Texture::loadFromImage(std::string path, std::string type_name) {
  glGenTextures(1, &id_);
  // stbi_set_flip_vertically_on_load(true);

  int width, height, nb_components;
  unsigned char* data =
      stbi_load(path.c_str(), &width, &height, &nb_components, 0);

  if (data) {
    GLenum format;
    switch (nb_components) {
      case 1:
        format = GL_RED;
        break;

      case 3:
        format = GL_RGB;
        break;

      case 4:
        format = GL_RGBA;
        break;
    }

    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::cout << "Texture : " << path << " loaded" << std::endl;
    std::cout << width << " " << height << " "
              << " " << type_name << " " << nb_components << std::endl;

    stbi_image_free(data);

  } else {
    std::cout << "Texture loading failed : " << path << std::endl;
    stbi_image_free(data);
  }

  type_name_ = type_name;
  path_ = path;
}

// void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
// {
// 	// Gets the location of the uniform
// 	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
// 	// Shader needs to be activated before changing the value of a uniform
// 	shader.Activate();
// 	// Sets the value of the uniform
// 	glUniform1i(texUni, unit);
// }

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, id_); }

void Texture::unbind() { glBindTexture(target_, 0); }

void Texture::clean() { glDeleteTextures(1, &id_); }

GLuint Texture::getId() { return id_; }

std::string Texture::getPath() { return path_; }

std::string Texture::getTypeName() { return type_name_; }

void Texture::loadDDSFile(std::string filepath, std::string type_name) {
  unsigned char header[124];

  FILE* fp;

  fp = fopen(filepath.c_str(), "rb");
  if (fp == NULL) {
    return;
  }

  char filecode[4];
  fread(filecode, 1, 4, fp);
  if (strncmp(filecode, "DDS ", 4) != 0) {
    fclose(fp);
    return;
  }

  fread(&header, 124, 1, fp);

  unsigned int height = *(unsigned int*)&(header[8]);
  unsigned int width = *(unsigned int*)&(header[12]);
  unsigned int linearSize = *(unsigned int*)&(header[16]);
  unsigned int mipMapCount = *(unsigned int*)&(header[24]);
  unsigned int fourCC = *(unsigned int*)&(header[80]);

  unsigned char* buffer;
  unsigned int bufsize;
  /* how big is it going to be including all mipmaps? */
  bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
  buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
  fread(buffer, 1, bufsize, fp);
  /* close the file pointer */
  fclose(fp);

  unsigned int components = (fourCC == MAKEFOURCC('D', 'X', 'T', '1')) ? 3 : 4;
  unsigned int format;
  switch (fourCC) {
    case MAKEFOURCC('D', 'X', 'T', '1'):
      format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
      break;
    case MAKEFOURCC('D', 'X', 'T', '3'):
      format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
      break;
    case MAKEFOURCC('D', 'X', 'T', '5'):
      format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
      break;
    default:
      free(buffer);
      return;
  }

  // Create one OpenGL texture
  glGenTextures(1, &id_);

  // "Bind" the newly created texture : all future texture functions will modify
  // this texture
  glBindTexture(GL_TEXTURE_2D, id_);

  unsigned int blockSize =
      (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
  unsigned int offset = 0;

  /* load the mipmaps */
  for (unsigned int level = 0; level < mipMapCount && (width || height);
       ++level) {
    unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
    glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size,
                           buffer + offset);

    offset += size;
    width /= 2;
    height /= 2;
  }
  free(buffer);

  type_name_ = type_name;
  path_ = filepath;
}