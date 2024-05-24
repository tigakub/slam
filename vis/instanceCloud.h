#ifndef __INSTANCECLOUD_H__
#define __INSTANCECLOUD_H__

template <typename InstanceData, class MeshClass>
class InstanceCloud: public UnmanagedGeometry<MeshClass> {
    protected:
        ShaderStorageBuffer<InstanceData> instanceSSBO;

    public:
        InstanceCloud(MeshClass & iMesh, GLuint iProgram)
        : UnmanagedGeometry(iMesh, iProgram) {
            static_assert(is_base_of<MeshBase, MeshClass>::value, "MeshClass not dericed from MeshBase");
        }

        InstanceCloud(InstanceCloud<MeshClass> && iOther)
        : UnmanagedGeometry(iOther) {
        }

        virtual ~InstanceCloud() { }

        virtual void update() {
            super.update();
        }

        virtual void draw() {
            super.draw();
        }
};

#endif
