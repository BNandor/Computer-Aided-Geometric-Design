#pragma once

#include <iostream>
#include <string>

namespace cagd
{
    class Exception
    {
        friend std::ostream& operator <<(std::ostream& lhs, const Exception& rhs);

    protected:
        std::string _reason;

    public:
        Exception(const std::string &reason): _reason(reason)
        {
        }
    };

    inline std::ostream& operator <<(std::ostream& lhs, const Exception& rhs)
    {
        return lhs << rhs._reason;
    }
}
