#include "Header.hpp"
#include "Message.hpp"
#include "request_utils.hpp"
#include "webserv.hpp"

static void   replace_spaces(string& resource);
static string get_field_value(stringstream& message);

void Header::parse_header(stringstream& message)
{
    parse_request_line(message);
    while (true && !message.eof() && !peek_terminator(message, CRLF))
        add_header(message);
    validate_terminator(message, CRLF);
    process_fields();
    if (state == BAD)
        message.setstate(std::ios::failbit);
}

void Header::parse_request_line(stringstream& message)
{
    static const string acceptedVersion("HTTP/1.1");
    char singleSpace[2] = { 0 };

    std::noskipws(message);
    message >> method >> singleSpace[0] >> resource >> singleSpace[1] >> version;
    if (strcmp(singleSpace, "  ") || acceptedVersion != version)
		state = BAD;
    validate_terminator(message, CRLF);
    replace_spaces(resource);
}

void Header::add_header(stringstream& message)
{
    static string fieldName;
    static string fieldValue;

    fieldName.clear();
    fieldValue.clear();

    std::getline(message, fieldName, ':');

    if (fieldName.find(' ') != string::npos)
		state = BAD;
    to_lower(fieldName);
    fieldValue = get_field_value(message);

    //! set-cookie is a special case where it would be on different fields
    if (fields.find(fieldName) != fields.end())
        fields.at(fieldName).append(", " + fieldValue);
    else
        fields.insert(std::make_pair(fieldName, fieldValue));
}

void Header::process_fields()
{
    // body size
    const HeaderMap::iterator encoding      = fields.find("transfer-encoding");
    const HeaderMap::iterator contentLength = fields.find("content-length");

    if (encoding != fields.end())
        is_chunked(encoding);
    else if (contentLength != fields.end())
        find_content_length(contentLength);
}

void Header::is_chunked(const HeaderMap::const_iterator it)
{
    const string& transferEncoding(it->second);
    string        last;
    stack<string> values;
    string        word;
    bool          exists(false);

    size_t delPos(0);
    size_t i(0);
    while (delPos != string::npos) { // not at end
        delPos = transferEncoding.find(", ", i);
        word   = transferEncoding.substr(i, delPos - i);
        to_lower(word); // transfer encodings are case insensitive
        values.push(word);
        i = delPos + 2;
        if (word == "chunked")
            exists = true;
    }
    // if chunked is the last encoding, set process chunked
    last = values.top();
    to_lower(last); // transfer encoding is case insensitive
    if (last == "chunked")
        chunked = true;
    // if not set status to 400, size cannot be determined
    if (exists && !chunked)
        state = BAD;
}

void Header::find_content_length(const HeaderMap::const_iterator it)
{
    if (it == fields.end()) {
        bodySize = NOT_SPECIFIED;
		return ;
	}

    istringstream length(it->second);
    length.exceptions(std::ios::failbit | std::ios::badbit);
    length >> bodySize;

    if (!length.eof()) {
        length.setstate(std::ios::failbit);
		// DEBUG_MSG("failed here", R);
	}
}

// helper utils
static string get_field_value(stringstream& message)
{
    string fieldValue;

    std::getline(message, fieldValue);

    string::size_type begin = fieldValue.find_first_not_of(" ");
    string::size_type end   = fieldValue.find_last_not_of(CRLF);

    if (begin != std::string::npos && end != std::string::npos)
        fieldValue = fieldValue.substr(begin, end - begin + 1);
    else if (begin != std::string::npos)
        fieldValue = fieldValue.substr(begin);

    return fieldValue;
}

// TODO replace all other special characters
static void replace_spaces(string& resource)
{
    size_t pos;
    while ((pos = resource.find("%20")) != std::string::npos)
        resource.replace(pos, 3, " ");
}
