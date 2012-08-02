#ifndef __gl_h_
	#if defined(__MACOSX__)
		#include <OpenGL/gl.h>
		#include <OpenGL/glext.h>
	#else
		#include <GL/gl.h>
		#include <GL/glext.h>
	#endif
#endif

#ifndef GL_FUNC
	#define GL_FUNC(type, name, args) \
		typedef type (APIENTRY * name##_p) args; \
		extern name##_p name;
	#define UNDEF_GL_FUNC
#endif

GL_FUNC(void, DeleteTextures, (GLsizei n, const GLuint *textures))
GL_FUNC(void, GenTextures, (GLsizei n, GLuint *textures))
GL_FUNC(void, BindTexture, (GLenum target, GLuint texture))
GL_FUNC(void, TexParameterf, (GLenum target, GLenum pname, GLfloat param))
GL_FUNC(void, TexImage2D, (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels))
GL_FUNC(void, Clear, (GLbitfield mask))
GL_FUNC(void, MatrixMode, (GLenum mode))
GL_FUNC(void, Viewport, (GLint x, GLint y, GLsizei width, GLsizei height))
GL_FUNC(void, LoadIdentity, (void))
GL_FUNC(void, Ortho, (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val))
GL_FUNC(void, Begin, (GLenum mode))
GL_FUNC(void, BlendFunc, (GLenum sfactor, GLenum dfactor))
//GL_FUNC(void, Color4f, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha))
GL_FUNC(void, Enable, (GLenum cap))
GL_FUNC(void, End, (void))
//GL_FUNC(GLboolean, IsTexture, (GLuint texture))
//GL_FUNC(void, PopMatrix, (void))
//GL_FUNC(void, PushMatrix, (void))
//GL_FUNC(void, Rotatef, (GLfloat angle, GLfloat x, GLfloat y, GLfloat z))
GL_FUNC(void, TexCoord2f, (GLfloat s, GLfloat t))
//GL_FUNC(void, Translatef, (GLfloat x, GLfloat y, GLfloat z))
GL_FUNC(void, Vertex2f, (GLfloat x, GLfloat y))
//GL_FUNC(void, EnableClientState, (GLenum state))
//GL_FUNC(void, ClearColor, (GLfloat r, GLfloat g, GLfloat b))
//GL_FUNC(void, Scalef, (GLfloat x, GLfloat y, GLfloat z))
GL_FUNC(void, LineWidth, (GLfloat w))
//GL_FUNC(void, AlphaFunc, (GLenum func, GLclampf ref))

#ifdef UNDEF_GL_FUNC
	#undef GL_FUNC
	#undef UNDEF_GL_FUNC
#endif
