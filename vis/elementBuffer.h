#ifndef __ELEMENTBUFFER_H__
#define __ELEMENTBUFFER_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include "usedsa.h"

using namespace std;
using namespace glm;

class ElementBufferBase {
    friend class VertexArray;
    
    protected:
        GLuint ebo;
        bool isDynamic;
        bool dirty;

    public:
        ElementBufferBase(bool iIsDynamic = false);
        virtual ~ElementBufferBase();

        virtual void init();
        #ifdef USEDSA
        void attachToVAO(GLuint iVao);
        #endif
        virtual void update();
        virtual void bind();
        virtual void unbind();

    protected:
        virtual const void *getData() const = 0;
        virtual GLuint getDataSize() const = 0;
        virtual GLsizei getElementSize() const = 0;
};

template <typename DataType>
class ElementBuffer: public ElementBufferBase {
    protected:
        vector<DataType> data;

    public:
        ElementBuffer(bool iIsDynamic = false)
        : ElementBufferBase(iIsDynamic) { }

        void addElement(DataType iElement) {
            data.push_back(iElement);
        }

    protected:
        virtual const void *getData() const { return (void *) &data[0]; }
        virtual GLuint getDataSize() const { return (GLuint) data.size(); }
        virtual GLsizei getElementSize() const { return (GLsizei) sizeof(DataType); }
};

#endif
