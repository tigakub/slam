#include "vertexArray.h"

VertexArray::VertexArray()
: vao(0) { }

VertexArray::~VertexArray() {
}


void VertexArray::init(ElementBufferBase & iEBuf, VertexBufferBase & iVBuf) {
    #ifdef USEDSA
        glCreateVertexArrays(1, &vao);
        
        GLuint i = 0;

        for(auto vBuf: vertexBuffers) {
            vBuf->init();

            vBuf->attachToVAO(vao, i, 0)
        }

        elementBuffer->attachToVAO(vao);
    #else
        glGenVertexArrays(1, &vao);
        bind();

        const BufferFormat & bufferFormat = iVBuf.getBufferFormat();

        iVBuf.init();
        GLuint i = 0;
        GLuint offset = 0;
        for(auto & vtxFmt: bufferFormat.format) {
            glVertexAttribPointer(i, vtxFmt.componentCount, vtxFmt.componentType, GL_FALSE, bufferFormat.byteStride, (void *) (uint64_t) offset );
            glEnableVertexAttribArray(i);
            i++;
            offset += vtxFmt.byteSize;
        }

        iEBuf.init();
    #endif
}

void VertexArray::cleanUp() {
    unbind();
    if(vao) glDeleteVertexArrays(1, &vao);

}

void VertexArray::bind() {
    glBindVertexArray(vao);
}

void VertexArray::unbind() {
    glBindVertexArray(0);
}
