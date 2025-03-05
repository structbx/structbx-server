#include "http/html_message.h"

std::string StructBX::HTTP::HTMLMessage(std::string code, std::string title, std::string message)
{
    std::string html = "<!DOCTYPE html>" \
        "<html lang=\"en\">" \
        "<head>" \
        "<meta charset=\"UTF-8\">" \
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" \
        "<title>" + title + " | StructBX</title>" \
        "<style>" \
        "body { background-color: #121212; color: #e0e0e0; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; text-align: center;}" \
        ".error-container { max-width: 600px; padding: 40px; background-color: #1e1e1e; border-radius: 8px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.5);}" \
        "h1 { color: #eee063; margin-bottom: 20px;}" \
        "p { line-height: 1.6; margin-bottom: 30px;}" \
        ".error-code { font-size: 3em; color: #eee063; margin-bottom: 20px;}" \
        ".back-link { color: #eee063; text-decoration: none; border: 1px solid #eee063; padding: 10px 20px; border-radius: 5px; transition: background-color 0.3s, color 0.3s;}" \
        ".back-link:hover { background-color: #eee063; color: #121212;}" \
        "</style>" \
        "</head>" \
        "<body>" \
        "<div class=\"error-container\">" \
        "<div class=\"error-code\">" + code + "</div>" \
        "<h1>" + title + "</h1>" \
        "<p>" + message + "</p>" \
        "<a href=\"/\" class=\"back-link\">Go to start</a>" \
        "</div>" \
        "</body>" \
        "</html>"
    ;

    return html;
}