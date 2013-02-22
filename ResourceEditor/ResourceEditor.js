#! /usr/bin/env node

var http = require('http');
var fs = require('fs');
var url = require('url');
var qs = require('querystring');

var __fakePath = 'fakepath'
var __resourcePath = '../Resources/iphone';
var __facesPath = '../Resources/iphone/faces';
var __spriteSheetPath = '../Resources/iphone/spritesheets';

String.prototype.startswith = function (s) {
    return s.length > 0 && this.indexOf(s) === 0;
}

String.prototype.endswith = function (s) { 
    var i = this.lastIndexOf(s);

    if (i === -1) {
        return false;
    }

    return this.length - i === s.length; 
}

String.prototype.stringByDeletingPathExtension = function () {
    var i = this.lastIndexOf('.');

    return this.substring(0, i);
}

function fakepathToRealPath(aFakePath) {
    return aFakePath.replace(__fakePath + '/', __resourcePath + '/')
}

var REService = {
    name: undefined,
    handler: function () {},
    argv: [],
    options: [],
    json: false,
    failedText: '',

    extend: function (options) {
        return {
            name: options.name,
            handler: typeof options.handler === 'function' ? options.handler : this.handler,
            argv: Array.isArray(options.argv) ? options.argv : this.argv,
            options: Array.isArray(options.options) ? options.options : this.options,
            json: options.json === true ? true : false,
            failedText: typeof options.failedText === 'string' ? options.failedText : this.failedText
        }
    }
}


    /*
     * 请求文件
     * 参数：
     *      path: 路径
     * 成功返回文件内容,否则返回false
     */
var requestFile = REService.extend({
        name: 'requestFile',
        handler: function (arguments) {
            var path = fakepathToRealPath(arguments.path);

            if (fs.existsSync(path)
                && fs.statSync(path).isFile()) {
                return fs.readFileSync(path);
            } else {
                return false;
            }
        },
        argv: ['path'],
    }),

    /*
     * 写入文件
     * 参数：
     *      path: 路径
     * 成功返回字符串ok,否则程序出错~~~~
     */
    writeFile = REService.extend({
        name: 'writeFile',
        handler: function (arguments) {
            var path = fakepathToRealPath(arguments.path);
            
            fs.writeFileSync(path, arguments.data);
            return 'ok';
        },
        argv: ['path', 'data'],
        failedText: '参数错误(path: ${path}, data: ${data})！'
    }),

    /*
     * 删除文件
     * 参数：
     *      path: 路径
     * 成功返回字符串done,否则返回false
     */
    deleteFile = REService.extend({
        name: 'deleteFile',
        handler: function (arguments) {
            var path = fakepathToRealPath(arguments.path);

            if (fs.existsSync(path)) {
                fs.unlinkSync(path);
                return 'done';
            } else {
                return false;
            }
        },
        argv: ['path'],
        failedText: 'failed'
    }),

    /*
     * 列出目录下文件
     * 参数：
     *      path: 路径
     *      extension(option): 筛选扩展名，以|隔开，例子：.wav|.mp3
     *      folder(option): 是否包含文件夹，默认是true
     * 返回筛选的文件
     */
    listDirectory = REService.extend({
        name: 'listDirectory',
        handler: function (arguments) {
            var path = fakepathToRealPath(arguments.path),
                folder = arguments.folder !== 'false' ? true : false,
                extension = arguments.extension,
                extensions,
                files,
                filtered = [];

            if (fs.existsSync(path)) {
                files = fs.readdirSync(path);
                if (extension) {
                    extensions = extension.split('|');
                    for (var i = 0; i < files.length; i++) {
                        for (var j = 0; j < extensions.length; ++j) {
                            if (extensions[j].length > 0 
                                && files[i].endswith(extensions[j])) {
                                filtered.push(files[i]);
                            }
                        }
                    }
                } else {
                    for (var i = 0; i < files.length; i++) {
                        if (!folder && fs.statSync(path + '/' + files[i]).isDirectory()) {
                            continue;
                        }
                        filtered.push(files[i]);
                    }
                }

                return filtered;
            } else {
                return null;
            }
        },
        argv: ['path'],
        options: ['extension', 'folder'],
        json: true,
        failedText: '[]'
    }),

    /* 特殊服务 */
    /*
     * 搜索返回符合格式的人脸文件
     * 返回人脸文件列表
     */
    faces = REService.extend({
        name: 'faces',
        handler: function (arguments) {
            var files = fs.readdirSync(__facesPath),
                faces = [];

            for (var i = 0; i < files.length; ++i) {
                if (files[i].endswith('.png')) {
                    faces.push(files[i]);
                }
            }

            return faces;
        },
        json: true
    }),

    /*
     * 搜索返回符合格式的精灵表
     * 返回精灵表列表
     */
    spriteSheet = REService.extend({
        name: 'spriteSheet',
        handler: function (arguments) {
            var files = fs.readdirSync(__spriteSheetPath),
                plists = [],
                spriteSheet = [],
                spriteSheetSet = {};

            /* 以plist文件为参考 */
            for (var i = 0; i < files.length; ++i) {
                if (files[i].endswith('.plist')) {
                    plists.push(files[i].stringByDeletingPathExtension());
                }
            }

            for (var i = 0; i < plists.length; ++i) {
                for (var j = 0; j < files.length; ++j) {
                    if (files[j].startswith(plists[i])
                        && !files[j].endswith('.plist')) {
                        if (!spriteSheetSet[plists[i]]) {
                            spriteSheet.push(plists[i]);
                        }
                        spriteSheetSet[plists[i]] = true;
                    }
                }
            }

            return spriteSheet;
        },
        json: true
    });

var services = {
    _services: {},
    _prefix: '/services/',
    init: function (callback, data) {
        callback(data);
    },
    install: function (aService) {
        this._services[aService.name] = aService;
    },
    uninstall: function (aService) {
        if (typeof aService === 'string') {
            delete this._services[aService];
        } else {
            delete this._services[aService.name];
        }
    },
    dispatch: function (aServiceName, aRequest, aResponse) {
        var body = '',
            service = this._services[aServiceName],
            postData,
            argumentName,
            arguments,
            result,
            pattern;

        if (! service) { /* 木有此服务 */
            return;
        }

        aRequest.on('data', function (data) {
            body += data;
        });
        aRequest.on('end', function () {
            postData = qs.parse(body);
            arguments = {};
            /* 组装必要参数 */
            for (var i = 0; i < service.argv.length; ++i) {
                argumentName = service.argv[i];
                if (! postData[argumentName]) { /* 参数不够 */
                    return;
                }
                arguments[argumentName] = postData[argumentName];
            }
            /* 组装可选参数 */
            for (var i = 0; i < service.options.length; ++i) {
                argumentName = service.options[i];
                if (! postData[argumentName]) { /* 没有使用此可选参数 */
                    continue;
                }
                arguments[argumentName] = postData[argumentName];
            }
            result = service.handler(arguments);
            if (result !== false || result != null) { /* 成功执行服务 */
                aResponse.end(service.json ? JSON.stringify(result) : result);
            } else {
                for (var i = 0; i < service.argv.length; ++i) {
                    argumentName = service.argv[i];
                    pattern = new RegExp('\\$\\{' + argumentName + '\\}', 'g');
                    failedText = failedText.replace(pattern, postData[argumentName]);
                }
                aResponse.end(service.failedText);
            }
        });
    }
}

services.init(function () {
    services.install(requestFile);
    services.install(writeFile);
    services.install(deleteFile);
    services.install(listDirectory);

    /* 安装特殊服务 */
    services.install(spriteSheet);
    services.install(faces);
});

http.createServer(function (req, res) {
    var url_ = req.url,
        requestObject = url.parse(url_,true,false),
        serviceName;

    if (req.method === 'POST') {
        if (requestObject.pathname.startswith(services._prefix)) {
            serviceName = requestObject.pathname.substring(10);
            services.dispatch(serviceName, req, res);
        }
    } else if (req.method === 'GET') {
        var path,
            fileData,
            contentType;

        if (requestObject.pathname === '/') {
            path = '/index.html';
        } else if (requestObject.pathname.startswith('/fakepath')) {
            path = fakepathToRealPath(requestObject.pathname)
        } else {
            path = requestObject.pathname;
        }

        path = path.substring(1);
        fileData = services._services.requestFile.handler({path:path});
        if (fileData === false) {
            fileData = services._services.requestFile.handler({path:'404.html'});
        }
        // console.log(path+': '+fileData.length)
        if (path.endswith('.js')) {
            contentType = 'text/javascript';
        } else if (path.endswith('.css')) {
            contentType = 'text/css';
        } else if (path.endswith('.html')) {
            contentType = 'text/html';
        } else if (path.endswith('.png')) {
            contentType = 'image/png';
        } else if (path.endswith('.jpg')) {
            contentType = 'image/jpeg';
        } else if (path.endswith('.bmp')) {
            contentType = 'image/bmp';
        } else if (path.endswith('.tmx')) {
            contentType = 'text/xml';
        } else if (path.endswith('.plist')) {
            contentType = 'application/x-plist';
        } else {
            // console.log(requestObject)
            contentType = 'text/plain';
        }
        res.writeHead(200, {
            'Content-Length': fileData.length,
            'Content-Type': contentType
        });
        res.end(fileData);
    }
}).listen(8080, '');
console.log('running on http://localhost:8080/');
