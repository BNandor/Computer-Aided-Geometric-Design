//#include "Texture.h"

//#include <Window/OpenGL.h>

//#include "FreeImage.h"

//namespace util {
//	FREE_IMAGE_FORMAT GetFileFormat(const char * filename) {
//		FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename, 0);
//		if (fif == FIF_UNKNOWN) {
//			fif = FreeImage_GetFIFFromFilename(filename);
//		}
//		return(fif);
//	}
//	FIBITMAP * GetFileContent(FREE_IMAGE_FORMAT fif, const char * filename) {
//		FIBITMAP * image = 0;
//		if (FreeImage_FIFSupportsReading(fif)) {
//			image = FreeImage_Load(fif, filename);
//		}
//		return(image);
//	}

//	Texture::Texture(const char * name, bool mipmap) {
//		FREE_IMAGE_FORMAT ext = GetFileFormat(name);
//		if (ext == FIF_UNKNOWN) {
//			throw("Invalid Texture File Format!");
//		}
//		FIBITMAP * content = GetFileContent(ext, name);
//		if (!content) {
//			throw("Invalid Texture File Content!");
//		}
//		BYTE * data = FreeImage_GetBits(content);

//		height = FreeImage_GetHeight(content);
//		width = FreeImage_GetWidth(content);

//		int internal = FreeImage_GetBPP(content) / 8;
//		GLenum format = (internal == 4) ? GL_BGRA : GL_BGR;
//		GLenum type = GL_UNSIGNED_BYTE;

//		glGenTextures(1, &texture);
//		glBindTexture(GL_TEXTURE_2D, texture);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//		if (mipmap) {
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//			gluBuild2DMipmaps(GL_TEXTURE_2D, internal, width, height, format, type, data);
//		} else {
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, format, type, data);
//		}
//		FreeImage_Unload(content);

//	}

//	void Texture::Render(float x, float y, float scale, float angle, float px, float py) {
//		glPushAttrib(GL_ENABLE_BIT);
//		glEnable(GL_BLEND);
//		glEnable(GL_TEXTURE_2D);
//		glDisable(GL_FOG);
//		glDisable(GL_LIGHTING);
//		glDisable(GL_CULL_FACE);
//		glBindTexture(GL_TEXTURE_2D, texture);
//		glPushMatrix();
//		glTranslatef(x, y, 0);
//		glRotatef(angle, 0, 0, 1);
//		glScalef(scale, scale, 1);
//		glTranslatef(-px * width, -py * height, 0);
//		glColor3ub(255, 255, 255);
//		glBegin(GL_TRIANGLE_STRIP);
//		glTexCoord2f(0, 0);
//		glVertex2f(0, 0);
//		glTexCoord2f(0, 1);
//		glVertex2f(0, height);
//		glTexCoord2f(1, 0);
//		glVertex2f(width, 0);
//		glTexCoord2f(1, 1);
//		glVertex2f(width, height);
//		glEnd();
//		glPopMatrix();
//		glPopAttrib();
//	}

//	void Texture::Use() {
//		glBindTexture(GL_TEXTURE_2D, texture);
//	}

//	Texture::~Texture() {
//		glDeleteTextures(1, &texture);
//	}
//};
