#include "vertexArray.h"

VertexArray::VertexArray(ElementBufferBase & iEBuf, VertexBufferBase * iVBuf)
: elementBuffer(iEBuf), vertexBuffers() {
    vertexBuffers.push_back(iVBuf);
 }

VertexArray::~VertexArray() {
    unbind();
    if(vao) glDeleteVertexArrays(1, &vao);
}

void VertexArray::addVertexBuffer(VertexBufferBase * iVBuf) {
    vertexBuffers.push_back(iVBuf);
}

void VertexArray::init() {
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

        GLuint i = 0;

        for(auto vBuf: vertexBuffers) {
            vBuf->init();

            glVertexAttribPointer(i, vBuf->getComponentCount(), vBuf->getComponentType(), GL_FALSE, vBuf->getVertexSize(), (void *) 0 );
            glEnableVertexAttribArray(i);
            
            i++;
        }

        elementBuffer.init();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        unbind();
    #endif
}

void VertexArray::bind() {
    glBindVertexArray(vao);
}

void VertexArray::unbind() {
    glBindVertexArray(0);
}
