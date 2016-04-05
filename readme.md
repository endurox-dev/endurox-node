
# Install Node.js

Download the Node.js Linux 64-bit binaries and extract the file, for example into /opt:

https://nodejs.org/dist/v0.10.44/ (or any 0.10.* later)
```
$ cd /opt
$ wget https://nodejs.org/dist/v0.10.44/node-v0.10.44.tar.gz
$ tar -xzvf node-v0.10.44.tar.gz
```

Set PATH to include Node.js:

export PATH=/opt/node-v0.10.44/bin:$PATH

# Install EnduroX
1. Download Linux package (DEB/RPM/tar.gz) from http://www.endurox.org/projects/endurox/files and install with your OS tools.
2. To configure full EnduorX enviroment follow http://www.endurox.org/dokuwiki/doku.php?id=v2.4.x:guides:getting_started_tutorial guide.

# Starting EnduroX (depends on your project) and how system was built
```
$ cd /home/user1/projects/endurox
$ setndrx
$ xa start -y
```

# Listing services (these can be called from Node.js)
$ xa psc

# Building and running the addon project
```
$ git clone https://github.com/endurox-dev/endurox-node
$ cd endurox-node
$ npm install
# Run the test server:
$ node server
```

# Testing
Point your browser to `http://localhost:3008/tpcall'. This should call "UNIXINFO" service from EnduroX test001 suite.
