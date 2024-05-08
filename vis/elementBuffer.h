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
        bool isNew;
        bool dirty;

    public:
        ElementBufferBase(bool iIsDynamic = false);
        virtual ~ElementBufferBase();

        virtual size_t getCount() const = 0;
        virtual GLenum getPrimitiveType() const = 0;

        // virtual void init();
        #ifdef USEDSA
        void attachToVAO(GLuint iVao);
        #endif
        virtual void update();
        virtual void mark();
        virtual void unmark();
        virtual void bind();
        virtual void unbind();

    protected:
        virtual const void *getData() const = 0;
        virtual GLsizei getDataSize() const = 0;
        virtual GLsizei getElementSize() const = 0;
};

template <typename DataType, GLenum iPrimitiveType, bool iIsFixedSize = false, GLsizei iBufferSize = 0>
class ElementBuffer: public ElementBufferBase {
    protected:
        vector<DataType> data;

    public:
        ElementBuffer(bool iIsDynamic = false)
        : ElementBufferBase(iIsDynamic) { }

        virtual ~ElementBuffer() { }

        virtual size_t getCount() const { return data.size(); }
        virtual GLenum getPrimitiveType() const { return iPrimitiveType; }

        vector<DataType> &getElements() {
            return data;
        }

        void addElement(DataType iElement) {
            data.push_back(iElement);
        }

    protected:
        virtual const void *getData() const { return (void *) &data[0]; }
        virtual GLsizei getDataSize() const { return (GLsizei) (iIsFixedSize ? iBufferSize : data.size() * sizeof(DataType)); }
        virtual GLsizei getElementSize() const { return (GLsizei) sizeof(DataType); }
};

#endif
