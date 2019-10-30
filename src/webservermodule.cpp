#include "webservermodule.h"

void WebServerModule::uploadFile(String path)
{
    HTTPUpload& upload = server.upload();

    switch(upload.status)
    {
        case UPLOAD_FILE_START:
        {
            String filename = upload.filename;
            if (!filename.startsWith("/"))
                filename = "/" + filename;

            fileHandle = SPIFFS.open(filename, "w");
            break;
        }

        case UPLOAD_FILE_WRITE:
            fileHandle.write(upload.buf, upload.currentSize);
            break;

        case UPLOAD_FILE_END:
            if (fileHandle)
            {
                fileHandle.close();
                server.sendHeader("Location", "/success.html"); //Redirect to success page
                server.send(303);
            }
            else
            {
                server.send(500, "text/plain", "500: Could not create file");
            }
            break;
        case UPLOAD_FILE_ABORTED:
            break;
    }
}

String WebServerModule::removePath(String url)
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

String WebServerModule::getContentType(String path)
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

void WebServerModule::onFileUpload()
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

void WebServerModule::onWifiCredentialsReceived()
{
    Serial.println("Received password");
    Serial.println("SSID: " + server.arg("ssid"));

    for(int i = 0; i < server.args(); i++)
    {
        Serial.println(server.argName(i) + ": " + server.arg(i));
    }

//    send(new WifiInfo(server.arg(0), server.arg(1)));
    send<WifiInfo>("test", "test");

    server.sendHeader("Location", "/");
    server.send(303);
}

void WebServerModule::onResourceRequested()
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
    /*size_t size = */server.streamFile(file, contentType);
    
    file.close();
}

void WebServerModule::setupWebRequests()
{
    server.on("/login", HTTP_POST, [this]() {
        Serial.println("Posted to /login");
        onWifiCredentialsReceived();
    });

    server.on("/upload", HTTP_POST, [this]() {
        Serial.println("Downloading file");
        onFileUpload();
    });

    server.onNotFound([this]() {
        Serial.println("Resource requested");
        onResourceRequested();
    });
}

void WebServerModule::setup()
{
    //Setup file system
    if (!SPIFFS.begin())
    {
        Serial.println("Could not mount");
    }

    Serial.println("\tWebserver module");
    setupWebRequests();

    server.begin();
}

void WebServerModule::loop()
{
    delay(500);
    send<WifiInfo>("ssid test", "password test");

    //server.handleClient();
}