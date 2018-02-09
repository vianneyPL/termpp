#pragma once

#include <string>
#include <string_view>

namespace trm
{
struct line_iterator
{
    // implements advance
    // implements prompt limit
};

struct line
{
    using buffer_type = std::string;

    line(std::string prompt = ">> ")
        : _prompt(prompt)
        , _buffer(prompt)
        , _index(std::size(prompt))
    {}

    int advance(int n)
    {
        auto new_index = _index + n;
        if ((new_index >= std::size(_prompt)) && (new_index < (std::size(_buffer) + 1)))
        {
            _index = new_index;
        }
        return _index;
    }

    void reset() noexcept
    {
        _buffer = _prompt;
        _index  = std::size(_prompt);
    }

    void insert(char c) noexcept
    {
        _buffer.insert(_index, 1, c);
        ++_index;
    }

    void begin() noexcept
    {
        _index = std::size(_prompt) + 1;
    }

    void end() noexcept
    {
        _index = std::size(_buffer) + 1;
    }

    void del() noexcept
    {
        if (_index > std::size(_prompt))
        {
            _buffer.erase(_index - 1, 1);
            --_index;
        }
    }

    const std::string & buffer() const noexcept
    {
        return _buffer;
    }

    const std::string content() const noexcept
    {
        return _buffer.substr(std::size(_prompt));
    }

    std::string prompt() const noexcept
    {
        return std::string{_prompt};
    }

    int index() const noexcept
    {
        return _index;
    }

private:
    std::string _prompt;
    buffer_type _buffer;
    int _index;
};
} // namespace trm