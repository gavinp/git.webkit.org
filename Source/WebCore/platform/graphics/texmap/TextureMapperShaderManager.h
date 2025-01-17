/*
 Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies)

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 2 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
 */

#ifndef TextureMapperShaderManager_h
#define TextureMapperShaderManager_h

#if USE(ACCELERATED_COMPOSITING) && USE(TEXTURE_MAPPER)

#include "FloatQuad.h"
#include "IntSize.h"
#include "OpenGLShims.h"
#include "TextureMapper.h"
#include "TransformationMatrix.h"
#include <wtf/HashMap.h>
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>

namespace WebCore {

class BitmapTexture;
class TextureMapperShaderManager;

class TextureMapperShaderProgram : public RefCounted<TextureMapperShaderProgram> {
public:
    GLuint id() { return m_id; }
    GLuint vertexAttrib() { return m_vertexAttrib; }

    virtual ~TextureMapperShaderProgram();

    virtual void prepare(float opacity, const BitmapTexture*) { }
    GLint matrixVariable() const { return m_matrixVariable; }
    GLint sourceMatrixVariable() const { return m_sourceMatrixVariable; }
    GLint sourceTextureVariable() const { return m_sourceTextureVariable; }
    GLint opacityVariable() const { return m_opacityVariable; }

protected:
    void getUniformLocation(GLint& var, const char* name);
    void initializeProgram();
    virtual const char* vertexShaderSource() const = 0;
    virtual const char* fragmentShaderSource() const = 0;

    GLuint m_id;
    GLuint m_vertexAttrib;
    GLuint m_vertexShader;
    GLuint m_fragmentShader;
    GLint m_matrixVariable;
    GLint m_sourceMatrixVariable;
    GLint m_sourceTextureVariable;
    GLint m_opacityVariable;
};

class TextureMapperShaderProgramSimple : public TextureMapperShaderProgram {
public:
    static PassRefPtr<TextureMapperShaderProgramSimple> create();
    virtual void prepare(float opacity, const BitmapTexture*);

private:
    virtual const char* vertexShaderSource() const;
    virtual const char* fragmentShaderSource() const;
    TextureMapperShaderProgramSimple();
};

class TextureMapperShaderProgramOpacityAndMask : public TextureMapperShaderProgram {
public:
    static PassRefPtr<TextureMapperShaderProgramOpacityAndMask> create();
    virtual void prepare(float opacity, const BitmapTexture*);
    GLint maskTextureVariable() const { return m_maskTextureVariable; }

private:
    static int m_classID;
    virtual const char* vertexShaderSource() const;
    virtual const char* fragmentShaderSource() const;
    TextureMapperShaderProgramOpacityAndMask();
    GLint m_maskTextureVariable;
};

class TextureMapperShaderManager {
public:
    enum ShaderType {
        Invalid = 0, // HashMaps do not like 0 as a key.
        Simple,
        OpacityAndMask
    };

    TextureMapperShaderManager();
    virtual ~TextureMapperShaderManager();

    PassRefPtr<TextureMapperShaderProgram> getShaderProgram(ShaderType shaderType)
    {
        RefPtr<TextureMapperShaderProgram> program;
        if (shaderType == Invalid)
            return program;

        TextureMapperShaderProgramMap::iterator it = m_textureMapperShaderProgramMap.find(shaderType);
        if (it != m_textureMapperShaderProgramMap.end())
            return it->second;

        switch (shaderType) {
        case Simple:
            program = TextureMapperShaderProgramSimple::create();
            break;
        case OpacityAndMask:
            program = TextureMapperShaderProgramOpacityAndMask::create();
            break;
        }
        m_textureMapperShaderProgramMap.add(shaderType, program);
        return program;
    }

private:
    typedef HashMap<ShaderType, RefPtr<TextureMapperShaderProgram>, DefaultHash<int>::Hash, HashTraits<int> > TextureMapperShaderProgramMap;
    TextureMapperShaderProgramMap m_textureMapperShaderProgramMap;
};

}

#endif

#endif // TextureMapperShaderManager_h
