//
// Created by Egor Orachyov on 05.02.2018.
//

#ifndef OXYGEN_XTEXTURE_H
#define OXYGEN_XTEXTURE_H

class xTexture : public xResource
{
public:
    xTexture(char * name, char * path) : xResource(name, path)
    {
        char * imagepath = GetFilename();
        FIBITMAP * bitmap = NULL;

        FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
        fif = FreeImage_GetFileType(imagepath, 0);

        if (fif == FIF_UNKNOWN) {
            fif = FreeImage_GetFIFFromFilename(imagepath);
        }
        if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
            bitmap = FreeImage_Load(fif, imagepath);
        } else {
            printf("ERROR: Unknown formant for file %s \n", imagepath);
            exit(1);
        }
        if (FreeImage_GetBits(bitmap) == NULL) {
            printf("ERROR: Empty bitmap data container %s \n", imagepath);
            exit(1);
        }

        if (bitmap) {
            int width = FreeImage_GetWidth(bitmap);
            int height = FreeImage_GetHeight(bitmap);

            glGenTextures(1, &m_textureID);
            glBindTexture(GL_TEXTURE_2D, m_textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap));
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_BGR, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(bitmap));
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

            FreeImage_Unload(bitmap);
        } else {
            printf("ERROR: Cannot load bitmap %s \n", imagepath);
            exit(1);
        }
    }

    ~xTexture()
    {
        glDeleteTextures(1, &m_textureID);
    }

    GLuint GetTextureID()
    {
        return m_textureID;
    }

private:

    GLuint m_textureID;
};

#endif //OXYGEN_XTEXTURE_H
