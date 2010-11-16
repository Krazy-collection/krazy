/*
 * Copyright (C) 2008 Rivo Laks <rivolaks@hot.ee>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KGLLIB_GEOMETRYBUFFER_H
#define KGLLIB_GEOMETRYBUFFER_H

#include "kgllib.h"

#include <Eigen/Core>


namespace KGLLib
{

/**
 * @brief Utility class to represent format of a @ref GeometryBuffer.
 *
 * GeometryBufferFormat object encapsulates information about a GeometryBuffer
 *  object. That information includes which attributes (e.g. vertices, normals,
 *  texture coordinates) are stored in the buffer as well as the size (number
 *  of components) of each coordinate.
 **/
class KGLLIB_EXPORT GeometryBufferFormat
{
public:
    enum Format
    {
        Vertex3   = 1 << 0,
        Vertex4   = 1 << 1,
        Color3    = 1 << 2,
        Color4    = 1 << 3,
        Normal3   = 1 << 4,
        Normal    = Normal3,
        TexCoord2 = 1 << 5
    };

    /**
     * Creates an invalid format.
     *
     * You will need to specify at least vertex size and count before it can
     *  be used.
     **/
    GeometryBufferFormat();
    explicit GeometryBufferFormat(int vertexCount, int indexCount = 0);
    GeometryBufferFormat(Format fmt, int vertexCount, int indexCount = 0);

    /**
     * Sets number of vertex components to @p size.
     *
     * This has to be specified before the format is valid and can be used.
     **/
    void addVertices(int size)  { mVertexSize = size; }
    /**
     * Sets number of normal components to 3 (normals always have 3 components).
     **/
    void addNormals()  { mNormalSize = 3; }
    /**
     * Sets number of color components to @p size.
     **/
    void addColors(int size)  { mColorSize = size; }
    /**
     * Sets number of texture coordinate components to @p size.
     **/
    void addTexCoords(int size)  { mTexCoordSize = size; }

    /**
     * @return number of components in a vertex.
     **/
    int vertexSize() const  { return mVertexSize; }
    /**
     * @return number of components in a color.
     **/
    int colorSize() const  { return mColorSize; }
    /**
     * @return number of components in a normal.
     **/
    int normalSize() const  { return mNormalSize; }
    /**
     * @return number of components in a texture coordinate.
     **/
    int texCoordSize() const  { return mTexCoordSize; }

    /**
     * @return number of vertices.
     *
     * @see indexCount()
     **/
    int vertexCount() const  { return mVertexCount; }
    void setVertexCount(int count);
    /**
     * @return number of indices.
     *
     * If this is 0, then non-indexed rendering will be used and all vertices
     *  will be used in sequential order. Otherwise indexed rendering will be
     *  used and vertices will be used in order specified by indices.
     *
     * @see vertexCount(), isIndexed()
     **/
    int indexCount() const  { return mIndexCount; }
    void setIndexCount(int count);

    /**
     * @return whether this is an indexed format or not.
     *
     * @see indexCount()
     **/
    bool isIndexed() const  { return mIndexCount > 0; }

protected:
    void init(int vertexCount, int indexCount);

private:
    int mVertexCount;
    int mIndexCount;

    int mVertexSize;
    int mColorSize;
    int mNormalSize;
    int mTexCoordSize;  // TODO: support multiple texcoords
    // TODO: support vertex attributes
};


/**
 * @brief Collection of geometry data
 *
 * GeometryBuffer object holds vertex and index data and is able to render it.
 *  It tries to use the most efficient way for rendering.
 *
 * Usually you needn't use GeometryBuffer directly: it is used by Batch to
 *  store its data.
 *
 * If you want to create a shared GeometryBuffer and use it for multiple Batch
 *  objects, the easiest way is to use @ref Batch::createSharedBuffer() method.
 *
 * When you specify data for the buffer then an internal copy is made of that
 *  data. Thus you can safely delete the data after you have specified it.
 **/
class KGLLIB_EXPORT GeometryBuffer
{
public:
    /**
     * Creates new GeometryBuffer object, using the specified format.
     *
     * The GeometryBuffer type (subclass) that is used is chosen
     *  automatically depending on hardware features.
     **/
    static GeometryBuffer* createBuffer(const GeometryBufferFormat& format);
    /**
     * Creates new GeometryBuffer object, using the specified format and
     *  vertex/index counts.
     * This is a convenience method that can be used to avoid creation of the
     *  temporary GeometryBufferFormat object.
     **/
    static GeometryBuffer* createBuffer(GeometryBufferFormat::Format format, int vertexCount, int indexCount = 0);


    /**
     * Deletes this buffer and frees all allocated resources.
     **/
    virtual ~GeometryBuffer();

    /**
     * Renders entire contents of the buffer.
     * The buffer must be bound (by calling the bind() method) before it can
     *  be used.
     *
     * @see bind(), unbind(), render(int, int), renderIndexed()
     */
    virtual void render();
    /**
     * Renders a subset of indexed buffer data.
     * The buffer must be bound (by calling the bind() method) before it can
     *  be used.
     *
     * @param indices number of indices to use.
     * @param offset array index of the first index to use.
     *
     * @see render(), render(int, int), bind(), unbind()
     **/
    virtual void renderIndexedSubset(int indices, int offset) = 0;
    /**
     * Renders a subset of non-indexed buffer data.
     * The buffer must be bound (by calling the bind() method) before it can
     *  be used.
     *
     * @param indices number of vertices to use.
     * @param offset array index of the first vertex to use.
     *
     * @see render(), renderIndexed(), bind(), unbind()
     **/
    virtual void renderSubset(int vertices, int offset) = 0;

    /**
     * Binds the buffer. The buffer must be bound before you can use any of the
     *  render methods.
     *
     * @see unbind()
     **/
    virtual bool bind()  { return true; }
    /**
     * Unbinds the buffer.
     *
     * @see bind()
     **/
    virtual bool unbind()  { return true; }

    /**
     * Specifies vertex data to be used by this buffer.
     *
     * @param vertices pointer to the array of vertices.
     * @param count number of elements in the specified array.
     * @param offset offset of the first element in the buffer.
     *
     * This copies @p count elements from the specified array into the buffer.
     * The first element of the array will be copied to position @p offset in
     *  the buffer.
     *
     * Each element of the specified array must consist of the same number of
     *  floats as specified in the format (GeometryBufferFormat::vertexSize()).
     **/
    void addVertices(void* vertices, int count, int offset = 0);
    void addColors(void* colors, int count, int offset = 0);
    void addNormals(void* normals, int count, int offset = 0);
    void addTexCoords(void* texcoords, int count, int offset = 0);
    /**
     * Sets the indices array to @p indices. The array must contain at least
     *  @p count entries (if it contains more, then the remaining ones will be
     *  unused).
     **/
    virtual void addIndices(unsigned int* indices, int count, int offset = 0) = 0;

    /**
     * Sets the primitive type used to render this batch (e.g. GL_QUADS).
     *
     * Default value is GL_TRIANGLES.
     **/
    virtual void setPrimitiveType(GLenum type);
    /**
     * @return OpenGL primitive mode used to render this batch.
     **/
    GLenum primitiveType() const  { return mPrimitiveType; }

    /**
     * @return format of this buffer.
     **/
    const GeometryBufferFormat& format() const  { return mFormat; }

protected:
    GeometryBuffer(const GeometryBufferFormat& format);

    virtual void init(const GeometryBufferFormat& format);

    /**
     * Calculates size of the buffer (in bytes) according to current @ref format.
     **/
    int bufferSize() const;
    /**
     * Calculates size of the index buffer (in bytes) according to current @ref format.
     **/
    int indexBufferSize() const;

    /**
     * Writes data to the internal buffer.
     * @param data pointer to the data
     * @param size size of the data in bytes
     * @param offset offset of the internal buffer in bytes
     **/
    virtual void addData(void* data, int size, int offset) = 0;

protected:
    struct AttributeData
    {
        AttributeData(int _size, int _offset, int _stride)
        {
            size = _size;
            offset = _offset;
            stride = _stride;
        }
        AttributeData()
        {
            size = offset = stride = 0;
        }

        // Size of a single element, in bytes
        int size;
        // Offset of the first element in the buffer
        int offset;
        // Number of bytes between two consecutive elements
        int stride;
    };

    GLenum mPrimitiveType;
    GeometryBufferFormat mFormat;

    AttributeData mVertexData;
    AttributeData mColorData;
    AttributeData mNormalData;
    AttributeData mTexCoordData;
};

/**
 * @brief GeometryBuffer that uses vertex arrays.
 *
 * This is used on hardware that does not support VBOs (vertex buffer objects).
 **/
class GeometryBufferVertexArray : public GeometryBuffer
{
public:
    GeometryBufferVertexArray(const GeometryBufferFormat& format);
    virtual ~GeometryBufferVertexArray();

    virtual bool bind();
    virtual bool unbind();

    virtual void renderIndexedSubset(int indices, int offset);
    virtual void renderSubset(int vertices, int offset);

    virtual void addIndices(unsigned int* indices, int count, int offset = 0);

protected:
    // FIXME: ugly
    GeometryBufferVertexArray(const GeometryBufferFormat& format, bool createArrays);

    virtual void createArrays();
    virtual void addData(void* data, int size, int offset);

private:
    char* mBuffer;
    char* mIndexBuffer;
};

/**
 * @brief GeometryBuffer that uses VBOs.
 *
 * This is the fastest way of rendering but hardware support for VBOs (vertex
 *  buffer objects) is required.
 **/
class GeometryBufferVBO : public GeometryBufferVertexArray
{
public:
    GeometryBufferVBO(const GeometryBufferFormat& format);
    virtual ~GeometryBufferVBO();

    virtual bool bind();
    virtual bool unbind();

    virtual void addIndices(unsigned int* indices, int count, int offset = 0);

protected:
    virtual void createArrays();
    virtual void addData(void* data, int size, int offset);

private:
    GLuint mVBOId, mIndexVBOId;
};

}

#endif
