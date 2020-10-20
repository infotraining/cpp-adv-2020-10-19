#ifndef PARAGRAPH_HPP_
#define PARAGRAPH_HPP_

#include <cstdlib>
#include <cstring>
#include <iostream>

namespace Modernized
{
    class Paragraph
    {
        char* buffer_;

    protected:
        void swap(Paragraph& p) noexcept
        {
            std::swap(buffer_, p.buffer_);
        }

    public:
        Paragraph() : buffer_(new char[1024])
        {
            std::cout << "Paragraph()\n";
            std::strcpy(buffer_, "Default text!");
        }

        Paragraph(const char* txt) : buffer_(new char[1024])
        {
            std::cout << "Paragraph(" << txt << ")\n";
            std::strcpy(buffer_, txt);
        }

        Paragraph(const Paragraph& p) : buffer_(new char[1024])
        {
            std::strcpy(buffer_, p.buffer_);
            std::cout << "Paragraph(cc: " << buffer_ << ")\n";
        }

        Paragraph& operator=(const Paragraph& p)
        {
            Paragraph temp(p);
            swap(temp);

            return *this;
        }
        
        Paragraph(Paragraph&& source) noexcept : buffer_{source.buffer_}
        {
            source.buffer_ = nullptr;
            std::cout << "Paragraph(mv: " << buffer_ << ")\n";
        }
        
        Paragraph& operator=(Paragraph&& source) noexcept
        {
            if (this != &source)
            {
                delete[] buffer_;

                this->buffer_ = source.buffer_;
                source.buffer_ = nullptr;
            }
            
            return *this;
        }

        void set_paragraph(const char* txt)
        {
            std::strcpy(buffer_, txt);
        }

        const char* get_paragraph() const noexcept
        {
            return buffer_;
        }

        void render_at(int posx, int posy) const
        {
            std::cout << "Rendering text '" << buffer_ << "' at: [" << posx << ", " << posy << "]" << std::endl;
        }

        ~Paragraph() noexcept
        {
            std::cout << "~Paragraph(" << (buffer_ ? buffer_ : "after move") << ")\n";
            delete[] buffer_;
        }
    };
}

class Shape
{
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;    
};

class Text : public Shape
{
    int x_, y_;
    Modernized::Paragraph p_;
public:
    Text(int x, int y, const std::string& text) : x_{x}, y_{y}, p_{text.c_str()}
    {}

    void draw() const override
    {
        p_.render_at(x_, y_);
    }

    std::string text() const
    {
        const char* txt = p_.get_paragraph(); 
        return (txt == nullptr) ? "" : txt;
    }

    void set_text(const std::string& text)
    {
        p_.set_paragraph(text.c_str());
    }
};

#endif /*PARAGRAPH_HPP_*/
