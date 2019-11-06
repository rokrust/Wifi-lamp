#include "modules/webservermodule.h"
#include "messages.h"
#include <WifiClient.h>

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
    for(int i = 0; i < server.args(); i++)
    {
        Serial.println(server.argName(i) + ": " + server.arg(i));
    }

    send<WifiInfo>(server.arg(0), server.arg(1));

    server.sendHeader("Location", "/");
    server.send(303);
}

// void WebServerModule::onEventStreamRequested()
// {
//     Serial.println("Event requested");

//     WiFiClient client = server.client();
//     client.println("HTTP/1.1 200 OK");
//     client.println("Content-Type: text/event-stream;charset=UTF-8");
//     client.println("Connection: keep-alive");
//     client.println("Access-Control-Allow-Origin: *");
//     client.println("Cache-Control: no-cache");
//     client.println();
//     client.flush();
// }

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

    server.on("/aplist", [this](){
        Serial.println("Sending wifi ap");
        
        server.send(200, "application/json", data);
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

    subscribe<WifiAp>(&WebServerModule::sendWifiApList);
}

void WebServerModule::loop()
{
    server.handleClient();
}

void WebServerModule::sendWifiApList(WifiAp *ap)
{
    Serial.println("Wifi received");
    
    data = "{\"ssid\": \"" + ap->ssid + "\", \"signalStrength\": \"" + ap->signalStrength + "\"}";
    Serial.print("json: "); Serial.println(data);

    // client.println("event: aplist");
    // client.println("Content-Type: text/event-stream");
    // client.print("data: {\"ssid\": \"");
    // client.print(ap->ssid);
    // client.print("\", \"signalStrength\": \"");
    // client.print(ap->signalStrength);
    // client.println("\"}");
    // client.println();
    // client.flush();
}