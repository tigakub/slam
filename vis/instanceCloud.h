#ifndef __INSTANCECLOUD_H__
#define __INSTANCECLOUD_H__

#include "geometry.h"
#include "shaderStorageBuffer.h"

template <typename InstanceDataType, class MeshClass, bool iInstanceBufferIsFixed = false, GLsizei iInstanceBufferSize = 0>
class InstanceCloud: public UnmanagedGeometry<MeshClass> {
    protected:
        ShaderStorageBuffer<InstanceDataType, PointScaleVertex::bufferFormat, iInstanceBufferIsFixed, iInstanceBufferSize> instanceSSBO;

    public:
        InstanceCloud(MeshClass & iMesh, GLuint iInstanceDataBindPoint, bool iInstanceDataIsDynamic = false, GLuint iProgram = 0)
        : UnmanagedGeometry<MeshClass>(iMesh, iProgram),
          instanceSSBO(iInstanceDataBindPoint, iInstanceDataIsDynamic) {
            static_assert(is_base_of<MeshBase, MeshClass>::value, "MeshClass not dericed from MeshBase");
        }

        InstanceCloud(InstanceCloud<InstanceDataType, MeshClass, iInstanceBufferIsFixed, iInstanceBufferSize> && iOther)
        : UnmanagedGeometry<MeshClass>(iOther),
          instanceSSBO(::move(iOther.instanceSSBO)) {
        }

        virtual ~InstanceCloud() { }

        ShaderStorageBuffer<InstanceDataType, PointScaleVertex::bufferFormat, iInstanceBufferIsFixed, iInstanceBufferSize> &getInstanceDataBuffer() { return instanceSSBO; }

        virtual void update() {
            instanceSSBO.update();
            UnmanagedGeometry<MeshClass>::update();
        }

        virtual void draw() {
            if(this->program) {
                instanceSSBO.bind();
                glUseProgram(this->program);
                this->mesh.drawInstanced(instanceSSBO.getCount());
                glUseProgram(0);
                instanceSSBO.unbind();
            }
        }
};

#endif
