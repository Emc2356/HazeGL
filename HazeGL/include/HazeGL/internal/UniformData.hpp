#pragma once

#include <glad/glad.h>

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace HazeGL::internal {
    struct Uniform {
    public:
        Uniform(const std::string& name_, const int location_, const uint32_t type_) : 
        name(name_), location(location_), type(type_) {}
        Uniform() : 
        name(""), location(0), type(0) {}

        std::string name;
        int location;
        uint32_t type;

        inline void setUniform(const int&        v) const { glUniform1i(location, v); }
        inline void setUniform(const glm::ivec2& v) const { glUniform2i(location, v.x, v.y); }
        inline void setUniform(const glm::ivec3& v) const { glUniform3i(location, v.x, v.y, v.z); }
        inline void setUniform(const glm::ivec4& v) const { glUniform4i(location, v.x, v.y, v.z, v.w); }
        inline void setUniform(const bool&       v) const { glUniform1i(location, v); }
        inline void setUniform(const glm::bvec2& v) const { glUniform2i(location, v.x, v.y); }
        inline void setUniform(const glm::bvec3& v) const { glUniform3i(location, v.x, v.y, v.z); }
        inline void setUniform(const glm::bvec4& v) const { glUniform4i(location, v.x, v.y, v.z, v.w); }
        inline void setUniform(const float&      v) const { glUniform1f(location, v); }
        inline void setUniform(const glm::vec2&  v) const { glUniform2f(location, v.x, v.y); }
        inline void setUniform(const glm::vec3&  v) const { glUniform3f(location, v.x, v.y, v.z); }
        inline void setUniform(const glm::vec4&  v) const { glUniform4f(location, v.x, v.y, v.z, v.w); }
        inline void setUniform(const glm::mat4&  v) const { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(v)); }
        inline void setUniform(const double&     v) const { glUniform1d(location, v); }
        inline void setUniform(const glm::dvec2& v) const { glUniform2d(location, v.x, v.y); }
        inline void setUniform(const glm::dvec3& v) const { glUniform3d(location, v.x, v.y, v.z); }
        inline void setUniform(const glm::dvec4& v) const { glUniform4d(location, v.x, v.y, v.z, v.w); }
        inline void setUniform(const glm::dmat4& v) const { glUniformMatrix4dv(location, 1, GL_FALSE, glm::value_ptr(v)); }
        inline void setUniform(const uint32_t&   v) const { glUniform1ui(location, v); }
        inline void setUniform(const glm::uvec2& v) const { glUniform2ui(location, v.x, v.y); }
        inline void setUniform(const glm::uvec3& v) const { glUniform3ui(location, v.x, v.y, v.z); }
        inline void setUniform(const glm::uvec4& v) const { glUniform4ui(location, v.x, v.y, v.z, v.w); }
    };
}
