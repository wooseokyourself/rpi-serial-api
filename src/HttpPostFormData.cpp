#include "../include/HttpPostFormData.hpp"

/**
 * @return Number of fields.
 */
int HttpPostFormData::size () const {
    return this->contents.size();
}

/**
 * @brief Remove all fields.
 */
void HttpPostFormData::clear () {
    this->contents.clear();
}

/**
 * @brief Add one field. Invalid values are not be checked.
 * @param _contentType Content-Type value of multipart form. There should be no double quotes.
 * @param _contentName Content-Name value of multipart form. There should be no double quotes.
 * @param _content Content of multipart form.
 */
void HttpPostFormData::addField (const std::string _contentType, 
                                 const std::string _contentName, 
                                 const std::string _content) {
    MultipartContent content;
    content.type = _contentType;
    content.name = _contentName;
    content.data = _content;
    this->contents.push_back(content);
}

/**
 * @brief Change one field. Invalid values are not be checked.
 * @param i Index of target field.
 * @param _contentType Content-Type value of multipart form. There should be no double quotes.
 * @param _contentName Content-Name value of multipart form. There should be no double quotes.
 * @param _content Content of multipart form.
 * @return false if i is out of bounds.
 */
bool HttpPostFormData::changeField (const int i,
                                    const std::string _contentType, 
                                    const std::string _contentName, 
                                    const std::string _content) {
    if (i < 0 || this->size() <= i)
        return false;
    this->contents[i].type = _contentType;
    this->contents[i].name = _contentName;
    this->contents[i].data = _content;
    return true;
} 

/**
 * @return null if i is out of bounds.
 */
const std::string& HttpPostFormData::getContentType (const int i) const {
    if (i < 0 || this->size() <= i)
        return "null";
    return this->contents[i].type;
}

/**
 * @return null if i is out of bounds.
 */
const std::string& HttpPostFormData::getContentName (const int i) const {
    if (i < 0 || this->size() <= i)
        return "null";
    return this->contents[i].name;
}

/**
 * @return null if i is out of bounds.
 */
const std::string& HttpPostFormData::getContent (const int i) const {
    if (i < 0 || this->size() <= i)
        return "null";
    return this->contents[i].data;
}