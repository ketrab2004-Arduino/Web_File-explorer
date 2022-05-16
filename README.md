# Ethernet Shield File Explorer

![An Arduino with an Ethernet shield on it, standing on a large cooling block to cool the Arduino. And a medium cooling block balancing on a small cooling block sitting on the Ethernet shield's cpu being kept in place by 2 pieces of cardboard cut from a candy box.](./memorabilia/cooling_block_block_on_block.jpeg)
![Web interface showing what the folder where you put the files from the `sd` folder should more or less look like.](./memorabilia/web_web-folder.png)
<br/>

## Parts:
* Arduino UNO
* An Ethernet Shield (i used a bootleg)
* A microSD

## Setup:
1. Make sure your microSD has files and folders to explore
2. Add the files inside the `/sd` folder inside the project into a folder on the microSD
1. Attach the Ethernet shield onto the Arduino
1. Change your settings in reader.ino
    * `MAC_ADDRESS`: the mac address on the ethernet shield or [a generated one](https://www.browserling.com/tools/random-mac)
    (something like 5c:ec:46:80:8e:fa becomes 0x5C, 0xEC, 0x46, 0x80, 0x8E, 0xFA)
    * `IP_ADDRESS`: an available ip address on your network
    (you can see your ip address under IPv4 when you run `ipconfig` in cmd, then you can replace the last number with any number from 0-255)
    * `SD_CS`: port for your microSD
    * `WEB_FOLDER`: folder where you put the files in step 2 (make sure it's a 8.3 filename,
    so `/web/file-explorer` becomes `/WEB/FILE-E~1`)
    * `FILE_API_BUFFER_SIZE`: to change how many bytes should be used for the buffer to load files (256 leaves enough memory in my experience)
    * `DEBUG`: if you would like to see debug messages (like the headers of an incoming request)
    * `INFO`: if you would like to see info messages 
    (like when a user has connected, and what their ip is)
1. Upload the sketch
1. Go to the ip address on a device that is connected to the same network via ethernet
(it doesn't work for me over wifi, even though it's connected to the wifi router)
1. Explore :)
