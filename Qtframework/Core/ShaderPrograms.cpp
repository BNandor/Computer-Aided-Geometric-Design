#include "Exceptions.h"
#include <fstream>
#include "ShaderPrograms.h"

using namespace cagd;
using namespace std;

ShaderProgram::ShaderProgram():
        _vertex_shader(0), _fragment_shader(0), _program(0),
        _vertex_shader_file_name(""), _fragment_shader_file_name(""),
        _vertex_shader_source(""), _fragment_shader_source(""),
        _vertex_shader_compiled(0), _fragment_shader_compiled(0), _linked(0)
{
}

// returns GL_TRUE if an OpenGL error occurred, GL_FALSE otherwise.
GLboolean ShaderProgram::_ListOpenGLErrors(const char *file_name, GLint line, ostream& output) const
{
    GLenum gl_error;
    GLboolean result = GL_FALSE;

    gl_error = glGetError();
        output << "\t\\begin{OpenGL Errors}" << endl;
        
    while (gl_error != GL_NO_ERROR)
    {
        output << "\t\tError in file " << file_name << " at line " << line << ": " << endl << gluErrorString(gl_error) << endl;
        result = GL_TRUE;
        gl_error = glGetError();
    }
    
    output << "\t\\end{OpenGL Errors}" << endl << endl;
    return result;
}

GLvoid ShaderProgram::_ListVertexShaderInfoLog(ostream& output) const
{
    GLint info_log_length = 0;
    GLint chars_written  = 0;
    GLchar *info_log = 0;

    // check for OpenGL errors
    _ListOpenGLErrors(__FILE__, __LINE__, output);

    glGetShaderiv(_vertex_shader, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0)
    {
        info_log = new GLchar[info_log_length];
        if (!info_log)
            throw Exception("ShaderProgram::_ListVertexShaderInfoLog - Could not allocate information log buffer!");

        glGetShaderInfoLog(_vertex_shader, info_log_length, &chars_written, info_log);

        output << "\t\\begin{Vertex Shader Information Log}" << endl << "\t\tid = " << _vertex_shader << ", name = " << _vertex_shader_file_name << endl;
        output <<  "\t\t" << info_log << endl;
        output << "\t\\end{Vertex Shader Information Log}" << endl << endl;

        delete[] info_log;
    }

    // check for OpenGL errors
    _ListOpenGLErrors(__FILE__, __LINE__, output);
}

GLvoid ShaderProgram::_ListFragmentShaderInfoLog(ostream& output) const
{
    GLint info_log_length = 0;
    GLint chars_written  = 0;
    GLchar *info_log = 0;

    // check for OpenGL errors
    _ListOpenGLErrors(__FILE__, __LINE__, output);

    glGetShaderiv(_fragment_shader, GL_INFO_LOG_LENGTH, &info_log_length);

    if (info_log_length > 0)
    {
        info_log = new GLchar[info_log_length];
        if (!info_log)
            throw Exception("ShaderProgram::_ListFragmentShaderInfoLog - Could not allocate information log buffer!");

        glGetShaderInfoLog(_fragment_shader, info_log_length, &chars_written, info_log);

        output << "\t\\begin{Fragment Shader InfoLog}" << endl<< "\t\tid = " << _fragment_shader << ", name = "  << _fragment_shader_file_name << endl;
        output <<  "\t\t" << info_log << endl;
        output << "\t\\end{Fragment Shader InfoLog}" << endl << endl;

        delete[] info_log;
    }

    // check for OpenGL errors
    _ListOpenGLErrors(__FILE__, __LINE__, output);
}

GLvoid ShaderProgram::_ListProgramInfoLog(ostream& output) const
{
    GLint info_log_length = 0;
    GLint chars_written  = 0;
    GLchar *info_log = 0;

    // check for OpenGL errors
    _ListOpenGLErrors(__FILE__, __LINE__, output);

    glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &info_log_length);

    if (info_log_length > 0)
    {
        info_log = new GLchar[info_log_length];
        if (!info_log)
            throw Exception("ShaderProgram::_ListProgramInfoLog - Could not allocate information log buffer!");

        glGetProgramInfoLog(_program, info_log_length, &chars_written, info_log);

        output << "\t\\begin{Program InfoLog}" << endl << "\t\tid = " << _program << endl;
        output <<  "\t\t" << info_log << endl;
        output << "\t\\end{Program InfoLog}" << endl << endl;

        delete[] info_log;
    }

    // check for OpenGL errors
    _ListOpenGLErrors(__FILE__, __LINE__, output);
}

GLvoid ShaderProgram::_ListValidateInfoLog(ostream& output) const
{
    GLint status = GL_FALSE;

    // check for OpenGL errors
    _ListOpenGLErrors(__FILE__, __LINE__, output);

    glGetProgramiv(_program, GL_VALIDATE_STATUS, &status);

    output << "\t\\begin{Program Validate InfoLog}" << endl<< "\t\tid = " << _program << endl;
    output <<  (status? "\t\tValidated." : "\t\tNot validated.") << endl;
    output << "\t\\end{Program Validate InfoLog}" << endl << endl;

    // check for OpenGL errors
    _ListOpenGLErrors(__FILE__, __LINE__, output);
}

GLint ShaderProgram::GetUniformVariableLocation(const GLchar *name, GLboolean logging_is_enabled, ostream& output) const
{
    GLint loc = glGetUniformLocation(_program, name);

    if (loc == -1)
    {
        string reason = "\t\tNo such uniform named: ";
        reason += name;
        output << reason << endl;

        // check for OpenGL errors
        if (logging_is_enabled)
            _ListOpenGLErrors(__FILE__, __LINE__, output);
    }
    return loc;
}

GLboolean ShaderProgram::InstallShaders(const string &vertex_shader_file_name, const string &fragment_shader_file_name, GLboolean logging_is_enabled, std::ostream &output)
{
    // loading source codes into shader objects
    _vertex_shader_file_name = vertex_shader_file_name;
    _fragment_shader_file_name = fragment_shader_file_name;

    fstream vertex_shader_file(vertex_shader_file_name.c_str(), ios_base::in);

    if (!vertex_shader_file || !vertex_shader_file.good())
    {
        return GL_FALSE;
    }

    _vertex_shader_source = "";
    string aux;

    if (logging_is_enabled)
    {
        output << "Source of vertex shader" << endl;
        output << "-----------------------" << endl;
    }

    while (!vertex_shader_file.eof())
    {
        getline(vertex_shader_file, aux, '\n');
        _vertex_shader_source += aux + '\n';

        if (logging_is_enabled)
            output << "\t" << aux << endl;
    }

    vertex_shader_file.close();

    if (logging_is_enabled)
        output << endl;

    fstream fragment_shader_file(fragment_shader_file_name.c_str(), ios_base::in);

    if (!fragment_shader_file || !fragment_shader_file.good())
    {
        return GL_FALSE;
    }

    _fragment_shader_source = "";

    if (logging_is_enabled)
    {
        output << "Source of fragment shader" << endl;
        output << "-------------------------" << endl;
    }

    while (!fragment_shader_file.eof())
    {
            getline(fragment_shader_file, aux, '\n');
            _fragment_shader_source += aux + '\n';

            if (logging_is_enabled)
                output << "\t" << aux << endl;
    }

    fragment_shader_file.close();

    if (logging_is_enabled)
        output << endl;

    // 1) creating two empty shader objects
    {
        if (logging_is_enabled)
        {
            output << "Creating empty vertex and fragment shader objects..." << endl;
            output << "----------------------------------------------------" << endl;
        }

        _vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        _fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

        if (logging_is_enabled)
            output << "Done." << endl << endl;
    }

    // 2) setting the source codes for the shaders
    {
        if (logging_is_enabled)
        {
            output << "Setting the source codes for the shaders..." << endl;
            output << "-------------------------------------------" << endl;
        }

        const GLchar *pointer_to_vertex_shader_source = &_vertex_shader_source[0];
        glShaderSource(_vertex_shader, 1, &pointer_to_vertex_shader_source, NULL);

        const GLchar *pointer_to_fragment_shader_source = &_fragment_shader_source[0];
        glShaderSource(_fragment_shader, 1, &pointer_to_fragment_shader_source, NULL);

        if (logging_is_enabled)
        {
            // check for OpenGL errors
            _ListOpenGLErrors(__FILE__, __LINE__, output);

            output << "Done." << endl << endl;
        }
    }

    // 3) compiling the vertex shader
    {
        if (logging_is_enabled)
        {
            output << "Compiling the vertex shader..." << endl;
            output << "------------------------------" << endl;
        }

        glCompileShader(_vertex_shader);
        glGetShaderiv(_vertex_shader, GL_COMPILE_STATUS, &_vertex_shader_compiled);

        if (logging_is_enabled)
        {
            _ListVertexShaderInfoLog(output);
            output << (_vertex_shader_compiled ? "\tSuccessful." : "\tUnsuccessful.") << endl << "Done." << endl << endl;
        }

        if (!_vertex_shader_compiled)
        {
            glDeleteShader(_vertex_shader);
            return GL_FALSE;
        }
    }

    // 4) compiling the fragment shader
    {
        if (logging_is_enabled)
        {
            output << "Compiling the fragment shader..." << endl;
            output << "--------------------------------" << endl;
        }

        glCompileShader(_fragment_shader);
        glGetShaderiv(_fragment_shader, GL_COMPILE_STATUS, &_fragment_shader_compiled);

        if (logging_is_enabled)
        {
            _ListFragmentShaderInfoLog(output);
            output << (_fragment_shader_compiled ? "\tSuccessful." : "\tUnsuccessful.") << endl << "Done." << endl << endl;
        }

        if (!_fragment_shader_compiled)
        {
            glDeleteShader(_vertex_shader);
            glDeleteShader(_fragment_shader);
            return GL_FALSE;
        }
    }

    // 5) creating the program object
    {
        if (logging_is_enabled)
        {
            output << "Creating the program object..." << endl;
            output << "------------------------------" << endl;
        }

        _program = glCreateProgram();

        if (logging_is_enabled)
            output << "Done." << endl << endl;

        // attaching the vertex and fragment shaders to the program object
        if (logging_is_enabled)
            output << "\tAttaching vertex and fragment shaders to the program object..." << endl;

        glAttachShader(_program, _vertex_shader);
        glAttachShader(_program, _fragment_shader);

        // check for OpenGL errors
        if (logging_is_enabled)
        {
            _ListOpenGLErrors(__FILE__, __LINE__, output);
            output << "Done." << endl << endl;
        }

        // linking the program
        if (logging_is_enabled)
            output << "\tLinking the program..." << endl;

        glLinkProgram(_program);
        glGetProgramiv(_program, GL_LINK_STATUS, &_linked);

        if (logging_is_enabled)
        {
            _ListProgramInfoLog(output);
            output << (_linked ? "\tSuccessful." : "\tUnsuccessful.") << endl << "Done." << endl << endl;
        }

        if (!_linked)
        {
            glDeleteShader(_vertex_shader);
            glDeleteShader(_fragment_shader);
            glDeleteProgram(_program);
            return GL_FALSE;
        }
    }

    // 6) flag shaders for deletion
    {
        if (logging_is_enabled)
        {
            output << "Flag shaders for deletion..." << endl;
            output << "----------------------------" << endl;
        }

        glDeleteShader(_vertex_shader);
        glDeleteShader(_fragment_shader);

        output << "Done." << endl << endl;
    }

    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformVariable1f(const GLchar *name, GLfloat parameter) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniform1f(location, parameter);

    return GL_TRUE;
}


GLboolean ShaderProgram::SetUniformVariable2f(const GLchar *name, GLfloat parameter_1, GLfloat parameter_2) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
            return GL_FALSE;

    glUniform2f(location, parameter_1, parameter_2);

    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformVariable3f(const GLchar *name, GLfloat parameter_1, GLfloat parameter_2, GLfloat parameter_3) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
            return GL_FALSE;

    glUniform3f(location, parameter_1, parameter_2, parameter_3);

    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformVariable4f(const GLchar *name, GLfloat parameter_1, GLfloat parameter_2, GLfloat parameter_3, GLfloat parameter_4) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
            return GL_FALSE;

    glUniform4f(location, parameter_1, parameter_2, parameter_3, parameter_4);

    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformVariable1i(const GLchar *name, GLint parameter) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniform1i(location, parameter);

    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformVariable2i(const GLchar *name, GLint parameter_1, GLint parameter_2) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniform2i(location, parameter_1, parameter_2);

    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformVariable3i(const GLchar *name, GLint parameter_1, GLint parameter_2, GLint parameter_3) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniform3i(location, parameter_1, parameter_2, parameter_3);

    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformVariable4i(const GLchar *name, GLint parameter_1, GLint parameter_2, GLint parameter_3, GLint parameter_4) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniform4i(location, parameter_1, parameter_2, parameter_3, parameter_4);

    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformVariable1fv(const GLchar *name, GLsizei count, const GLfloat *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniform1fv(location, count, value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformVariable2fv(const GLchar *name, GLsizei count, const GLfloat *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniform2fv(location, count, value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformVariable3fv(const GLchar *name, GLsizei count, const GLfloat *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniform3fv(location, count, value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformVariable4fv(const GLchar *name, GLsizei count, const GLfloat *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniform4fv(location, count, value);
    return GL_TRUE;
}


GLboolean ShaderProgram::SetUniformVariable1iv(const GLchar *name, GLsizei count, const GLint *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniform1iv(location, count, value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformVariable2iv(const GLchar *name, GLsizei count, const GLint *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniform2iv(location, count, value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformVariable3iv(const GLchar *name, GLsizei count, const GLint *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniform3iv(location, count, value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformVariable4iv(const GLchar *name, GLsizei count, const GLint *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniform4iv(location, count, value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformMatrix2fv(const GLchar *name, GLsizei count, GLboolean transpose, const GLfloat *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniformMatrix2fv(location, count, transpose,value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformMatrix3fv(const GLchar *name, GLsizei count, GLboolean transpose, const GLfloat *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniformMatrix3fv(location, count, transpose,value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformMatrix4fv(const GLchar *name, GLsizei count, GLboolean transpose, const GLfloat *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniformMatrix4fv(location, count, transpose,value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformMatrix2x3fv(const GLchar *name, GLsizei count, GLboolean transpose, const GLfloat *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniformMatrix2x3fv(location, count, transpose,value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformMatrix3x2fv(const GLchar *name, GLsizei count, GLboolean transpose, const GLfloat *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniformMatrix3x2fv(location, count, transpose,value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformMatrix2x4fv(const GLchar *name, GLsizei count, GLboolean transpose, const GLfloat *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniformMatrix2x4fv(location, count, transpose,value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformMatrix4x2fv(const GLchar *name, GLsizei count, GLboolean transpose, const GLfloat *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniformMatrix4x2fv(location, count, transpose,value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformMatrix3x4fv(const GLchar *name, GLsizei count, GLboolean transpose, const GLfloat *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniformMatrix3x4fv(location, count, transpose,value);
    return GL_TRUE;
}

GLboolean ShaderProgram::SetUniformMatrix4x3fv(const GLchar *name, GLsizei count, GLboolean transpose, const GLfloat *value) const
{
    if (!_program)
        return GL_FALSE;

    GLint location = GetUniformVariableLocation(name);
    if (location == -1)
        return GL_FALSE;

    glUniformMatrix4x3fv(location, count, transpose,value);
    return GL_TRUE;
}

GLvoid ShaderProgram::Disable() const
{
    glUseProgram(0);
}

GLvoid ShaderProgram::Enable(GLboolean logging_is_enabled, ostream& output) const
{
    if (_vertex_shader_compiled && _fragment_shader_compiled && _linked)
    {
        glUseProgram(_program);
        glValidateProgram(_program);

        if (logging_is_enabled)
            _ListValidateInfoLog(output);
    }
}

ShaderProgram::~ShaderProgram()
{
    // all the attached shader objects will be automatically detached, and,
    // because they are flagged for deletion, they will be automatically deleted
    // at that time as well
    if (_program)
        glDeleteProgram(_program);
}
