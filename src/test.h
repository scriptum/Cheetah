/*
cat *.[hc] | grep -Eo '^\s*(gl[A-Z]|SDL_)[a-zA-Z0-9_]*' | awk '{print $1}' | sort -u | xargs -I {} echo "#define {}(...)"
* */

#ifndef TEST_H_
#define TEST_H_

#ifdef TEST_MODE

/* just reset, don't draw actually */

/*#undef FLUSH_BUFFER
#define FLUSH_BUFFER() do {                                                    \
    if(vertexCounter) {                                                        \
        vertexCounter = 0;                                                     \
    }                                                                          \
} while(0)
*/

//static inline bool alwaysTrue(void)
//{
    //return 1;
//}

//#define cheetahInit(...) alwaysTrue()
//#define isInit(...) alwaysTrue()


#define glActiveTexture_(...)
#define glAlphaFunc(...)
#define glAttachObject_(...)
#define glBegin(...)
#define glBindAttribLocation_(...)
#define glBindBuffer_(...)
#define glBindFramebuffer_(...)
#define glBindRenderbuffer_(...)
#define glBindTexture(...)
#define glBlendEquation_(...)
#define glBlendFunc(...)
#define glBufferData_(...)
#define glBufferSubData_(...)
#define glCallList(...)
#define glClear(...)
#define glClearColor(...)
#define glClientActiveTexture_(...)
#define glColor4ubv(...)
#define glColorMask(...)
#define glColorPointer(...)
#define glCompileShader_(...)
#define glCreateProgramObject_(...)
#define glCreateShaderObject_(...)
#define glDeleteBuffers_(...)
#define glDeleteFramebuffers_(...)
#define glDeleteObject_(...)
#define glDeleteRenderbuffers_(...)
#define glDeleteTextures(...)
#define glDisable(...)
#define glDisableClientState(...)
#define glDrawArrays(...)
#define glEnable(...)
#define glEnableClientState(...)
#define glEnd(...)
#define glEndList(...)
#define glFinish(...)
#define glFramebufferRenderbuffer_(...)
#define glFramebufferTexture2D_(...)
#define glGenBuffers_(...)
#define glGenerateMipmap_(...)
#define glGenFramebuffers_(...)
#define glGenRenderbuffers_(...)
#define glGenTextures(...)
#define glGetActiveUniform_(...)
#define glGetBufferSubData_(...)
#define glGetInfoLog_(...)
#define glGetIntegerv(...)
#define glGetObjectParameteriv_(...)
#define glGetProgramiv_(...)
#define glGetShaderiv_(...)
#define glGetUniformLocation_(...)
#define glLinkProgram_(...)
#define glLoadIdentity(...)
#define glMapBuffer_(...)
#define glMatrixMode(...)
#define glMultiTexCoord2f_(...)
#define glMultiTexCoord3f_(...)
#define glMultiTexCoord4f_(...)
#define glNewList(...)
#define glOrtho(...)
#define glPointSize(...)
#define glPopMatrix(...)
#define glPushMatrix(...)
#define glReadBuffer(...)
#define glReadPixels(...)
#define glRenderbufferStorage_(...)
#define glRotated(...)
#define glScaled(...)
#define glScalef(...)
#define glScissor(...)
#define glShaderSource_(...)
#define glStencilFunc(...)
#define glTexCoordPointer(...)
#define glTexEnvi(...)
#define glTexImage2D(...)
#define glTexParameteri(...)
#define glTranslated(...)
#define glTranslatef(...)
#define glUniform1f_(...)
#define glUniform1fv_(...)
#define glUniform1i_(...)
#define glUniform2f_(...)
#define glUniform2fv_(...)
#define glUniform3f_(...)
#define glUniform3fv_(...)
#define glUniform4f_(...)
#define glUniform4fv_(...)
#define glUnmapBuffer_(...)
#define glUseProgramObject_(...)
#define glVertex2f(...)
#define glVertexPointer(...)
#define glViewport(...)
#define SDL_CreateThread(...)
#define SDL_Delay(...)
#define SDL_DestroyWindow(...)
#define SDL_GetMouseState(...)
#define SDL_GetWindowSize(...)
#define SDL_GLContext(...)
#define SDL_GL_DeleteContext(...)
#define SDL_GL_SetAttribute(...)
#define SDL_GL_SetSwapInterval(...)
#define SDL_GL_SwapWindow(...)
#define SDL_main(...)
//#undef SDL_mutexP
//#define SDL_mutexP(...)
//#undef SDL_mutexV
//#define SDL_mutexV(...)
#define SDL_Quit(...)
#define SDL_SetModuleHandle(...)
#define SDL_SetWindowSize(...)
#define SDL_SetWindowTitle(...)
#define SDL_strlcat(...)
#define SDL_strlcpy(...)
#define SDL_Window(...)



#endif

#endif /* TEST_H_ */
