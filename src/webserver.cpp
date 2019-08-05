#include "webserver.h"


void WebServer::uploadFile(String path)
{
    HTTPUpload& upload = server.upload();

    if(upload.status == UPLOAD_FILE_START)
    {
        String filename = upload.filename;
        if(!filename.startsWith("/")) filename = "/" + filename;

        fileHandle = SPIFFS.open(filename, "w");
    }
    else if(upload.status == UPLOAD_FILE_WRITE)
    {
        if(fileHandle)
        {
            fileHandle.write(upload.buf, upload.currentSize);
        }
    }
    else if(upload.status == UPLOAD_FILE_END)
    {
        if(fileHandle)
        {
            fileHandle.close();
            server.sendHeader("Location", "/success.html"); //Redirect to success page
            server.send(303);
        }
        else
        {
            server.send(500, "text/plain", "500: Could not create file");
        }
    }
}

String WebServer::removePath(String url)
{
    for(int i = url.length() - 1; i > 0; i--)
    {
        if(url[i] == '/')
        {
            return url.substring(i);
        }
    }
    return url;
}

String WebServer::getContentType(String path)
{
    if(path.endsWith(".htm")) return "text/html";
    else if(path.endsWith(".html")) return "text/html";
    else if(path.endsWith(".css")) return "text/css";
    else if(path.endsWith(".js")) return "application/javascript";
    else if(path.endsWith(".png")) return "image/png";
    else if(path.endsWith(".gif")) return "image/gif";
    else if(path.endsWith(".jpg")) return "image/jpeg";
    else if(path.endsWith(".ico")) return "image/x-icon";
    else if(path.endsWith(".xml")) return "text/xml";
    else if(path.endsWith(".pdf")) return "application/x-pdf";
    else if(path.endsWith(".svg")) return "image/svg+xml";
    else if(path.endsWith(".zip")) return "application/x-zip";
    else if(path.endsWith(".gz")) return "application/x-gzip";
    return "text/plain";
}

WebServer::WebServer()
{
    server.on("/login", HTTP_POST, [this](){
        onWifiCredentialsReceived();
    });

    server.on("/upload", HTTP_POST, [this](){
        onFileUpload();
    });

    server.onNotFound([this](){
        onResourceRequested();
    });

}

void WebServer::onFileUpload()
{
    String path = server.uri();

    if(SPIFFS.exists(path))
    {
        uploadFile(path);
    }

    else
    {
        Serial.println("File " + path + " does not exist");
    }
}

void WebServer::onWifiCredentialsReceived()
{
    Serial.println("Received password");
    Serial.println("SSID: " + server.arg("ssid"));

    for(int i = 0; i < server.args(); i++)
    {
        Serial.println(server.argName(i) + ": " + server.arg(i));
    }

    server.sendHeader("Location", "/");
    server.send(303);
}

void WebServer::onResourceRequested()
{
    //Look for the requested file in the build folder
    String path = removePath(server.uri());
    path = "/build" + path;
    if(path.endsWith("/")) path += "index.html";
    
    //Exit if the file could not be found (Send error code?)
    if(!SPIFFS.exists(path)) 
    {
        Serial.println("Requested file " + String(path) + " could not be found");
        return;
    }

    //If the file exists, open it and stream it to the client
    String contentType = getContentType(path);
    File file = SPIFFS.open(path, "r");
    size_t size = server.streamFile(file, contentType);
    file.close();
}