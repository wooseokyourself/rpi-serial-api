#ifndef __HTTPPOSTFORMDATA__
#define __HTTPPOSTFORMDATA__

#include <vector>
#include <string>

class HttpPostFormData {
public:
    struct MultipartContent {
        std::string type;
        std::string name;
        std::string data;
    };

public:
    int size () const;
    void clear ();
    void addField (const std::string _contentType, 
                   const std::string _contentName, 
                   const std::string _content);
    bool changeField (const int i,
                      const std::string _contentType, 
                      const std::string _contentName, 
                      const std::string _content);
    const std::string& getContentType (const int i) const;
    const std::string& getContentName (const int i) const;
    const std::string& getContent (const int i) const;

private:
    std::vector<MultipartContent> contents;
};

#endif