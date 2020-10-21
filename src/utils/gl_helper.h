#ifndef BRAVE2_GL_HELPER_H
#define BRAVE2_GL_HELPER_H

#include <GL/glew.h>

// TODO should I change my naming?
#include "math.h"

//---------------------------
class Texture {
//---------------------------
    std::vector<vec4> load(std::string pathname, bool transparent, int& width, int& height) {
        FILE * file = fopen(pathname.c_str(), "r");
        if (!file) {
            printf("%s does not exist\n", pathname.c_str());
            width = height = 0;
            return std::vector<vec4>();
        }
        unsigned short bitmapFileHeader[27];					// bitmap header
        fread(&bitmapFileHeader, 27, 2, file);
        if (bitmapFileHeader[0] != 0x4D42) printf("Not bmp file\n");
        if (bitmapFileHeader[14] != 24) printf("Only true color bmp files are supported\n");
        width = bitmapFileHeader[9];
        height = bitmapFileHeader[11];
        unsigned int size = (unsigned long)bitmapFileHeader[17] + (unsigned long)bitmapFileHeader[18] * 65536;
        fseek(file, 54, SEEK_SET);
        std::vector<unsigned char> bImage(size);
        fread(&bImage[0], 1, size, file); 	// read the pixels
        fclose(file);
        std::vector<vec4> image(width * height);
        int i = 0;
        for (unsigned int idx = 0; idx < size; idx += 3) { // Swap R and B since in BMP, the order is BGR
            float alpha = (transparent) ? (bImage[idx] + bImage[idx + 1] + bImage[idx + 2]) / 3.0f / 256.0f : 1.0f;
            image[i++] = vec4(bImage[idx + 2] / 256.0f, bImage[idx + 1] / 256.0f, bImage[idx] / 256.0f, alpha);
        }
        return image;
    }

public:
    unsigned int textureId = 0;

    Texture() { textureId = 0; }

    Texture(std::string pathname, bool transparent = false) {
        textureId = 0;
        create(pathname, transparent);
    }

    Texture(int width, int height, const std::vector<vec4>& image, int sampling = GL_LINEAR) {
        textureId = 0;
        create(width, height, image, sampling);
    }

    Texture(const Texture& texture) {
        printf("\nError: Texture resource is not copied on GPU!!!\n");
    }

    void operator=(const Texture& texture) {
        printf("\nError: Texture resource is not copied on GPU!!!\n");
    }

    void create(std::string pathname, bool transparent = false) {
        int width, height;
        std::vector<vec4> image = load(pathname, transparent, width, height);
        if (image.size() > 0) create(width, height, image);
    }

    void create(int width, int height, const std::vector<vec4>& image, int sampling = GL_LINEAR) {
        if (textureId == 0) glGenTextures(1, &textureId);  				// id generation
        glBindTexture(GL_TEXTURE_2D, textureId);    // binding

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, &image[0]); // To GPU
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampling); // sampling
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampling);
    }

    ~Texture() {
        if (textureId > 0) glDeleteTextures(1, &textureId);
    }
};

//---------------------------
class GPUProgram {
//--------------------------
    unsigned int shaderProgramId = 0;
    unsigned int vertexShader = 0, geometryShader = 0, fragmentShader = 0;
    bool waitError = true;

    void getErrorInfo(unsigned int handle) { // shader error report
        int logLen, written;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            std::string log(logLen, '\0');
            glGetShaderInfoLog(handle, logLen, &written, &log[0]);
            printf("Shader log:\n%s", log.c_str());
            if (waitError) getchar();
        }
    }

    bool checkShader(unsigned int shader, std::string message) { // check if shader could be compiled
        int OK;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &OK);
        if (!OK) {
            printf("%s!\n", message.c_str());
            getErrorInfo(shader);
            return false;
        }
        return true;
    }

    bool checkLinking(unsigned int program) { 	// check if shader could be linked
        int OK;
        glGetProgramiv(program, GL_LINK_STATUS, &OK);
        if (!OK) {
            printf("Failed to link shader program!\n");
            getErrorInfo(program);
            return false;
        }
        return true;
    }

    int getLocation(const std::string& name) {	// get the address of a GPU uniform variable
        int location = glGetUniformLocation(shaderProgramId, name.c_str());
        if (location < 0) printf("uniform %s cannot be set\n", name.c_str());
        return location;
    }

public:
    GPUProgram(bool _waitError = true) { shaderProgramId = 0; waitError = _waitError; }

    GPUProgram(const GPUProgram& program) {
        if (program.shaderProgramId > 0) printf("\nError: GPU program is not copied on GPU!!!\n");
    }

    void operator=(const GPUProgram& program) {
        if (program.shaderProgramId > 0) printf("\nError: GPU program is not copied on GPU!!!\n");
    }

    unsigned int getId() { return shaderProgramId; }

    bool create(const char * const vertexShaderSource,
                const char * const fragmentShaderSource, const char * const fragmentShaderOutputName,
                const char * const geometryShaderSource = nullptr)
    {
        // Create vertex shader from string
        if (vertexShader == 0) vertexShader = glCreateShader(GL_VERTEX_SHADER);
        if (!vertexShader) {
            printf("Error in vertex shader creation\n");
            exit(1);
        }
        glShaderSource(vertexShader, 1, (const GLchar**)&vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        if (!checkShader(vertexShader, "Vertex shader error")) return false;

        // Create geometry shader from string if given
        if (geometryShaderSource != nullptr) {
            if (geometryShader == 0) geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
            if (!geometryShader) {
                printf("Error in geometry shader creation\n");
                exit(1);
            }
            glShaderSource(geometryShader, 1, (const GLchar**)&geometryShaderSource, NULL);
            glCompileShader(geometryShader);
            if (!checkShader(geometryShader, "Geometry shader error")) return false;
        }

        // Create fragment shader from string
        if (fragmentShader == 0) fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        if (!fragmentShader) {
            printf("Error in fragment shader creation\n");
            exit(1);
        }

        glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        if (!checkShader(fragmentShader, "Fragment shader error")) return false;

        shaderProgramId = glCreateProgram();
        if (!shaderProgramId) {
            printf("Error in shader program creation\n");
            exit(1);
        }
        glAttachShader(shaderProgramId, vertexShader);
        glAttachShader(shaderProgramId, fragmentShader);
        if (geometryShader > 0) glAttachShader(shaderProgramId, geometryShader);

        // Connect the fragmentColor to the frame buffer memory
        glBindFragDataLocation(shaderProgramId, 0, fragmentShaderOutputName);	// this output goes to the frame buffer memory

        // program packaging
        glLinkProgram(shaderProgramId);
        if (!checkLinking(shaderProgramId)) return false;

        // make this program run
        glUseProgram(shaderProgramId);
        return true;
    }

    void Use() { 		// make this program run
        glUseProgram(shaderProgramId);
    }

    void setUniform(int i, const std::string& name) {
        int location = getLocation(name);
        if (location >= 0) glUniform1i(location, i);
    }

    void setUniform(float f, const std::string& name) {
        int location = getLocation(name);
        if (location >= 0) glUniform1f(location, f);
    }

    void setUniform(const vec2& v, const std::string& name) {
        int location = getLocation(name);
        if (location >= 0) glUniform2fv(location, 1, &v.x);
    }

    void setUniform(const vec3& v, const std::string& name) {
        int location = getLocation(name);
        if (location >= 0) glUniform3fv(location, 1, &v.x);
    }

    void setUniform(const vec4& v, const std::string& name) {
        int location = getLocation(name);
        if (location >= 0) glUniform4fv(location, 1, &v.x);
    }

    void setUniform(const mat4& mat, const std::string& name) {
        int location = getLocation(name);
        if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, mat);
    }

    void setUniform(const Texture& texture, const std::string& samplerName, unsigned int textureUnit = 0) {
        int location = getLocation(samplerName);
        if (location >= 0) {
            glUniform1i(location, textureUnit);
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, texture.textureId);
        }
    }

    ~GPUProgram() { if (shaderProgramId > 0) glDeleteProgram(shaderProgramId); }
};

#endif //BRAVE2_GL_HELPER_H
