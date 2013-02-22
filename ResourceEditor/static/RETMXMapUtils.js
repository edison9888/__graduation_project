
(function (window_) {
    var rootPath = REResourceGetMCResourcesPath(''),
        spritesheetsPath = REResourceGetMCResourcesPath('spritesheets'),
        soundsPath = REResourceGetMCResourcesPath('sounds'),
        mapPath = REResourceGetMCResourcesPath('maps');

    function REResourcesFilter (data) {
        var resources = [],
            resource;

        for (var i = 0, len = data.length; i < len; ++i) {
            resource = data[i];
            if (resource.endswith('.png')
                || resource.endswith('.jpg')
                || resource.endswith('.jpeg')
                || resource.endswith('.gif')
                || resource.endswith('.bmp')
                || resource.endswith('.mp3')
                || resource.endswith('.wav')
                || resource.endswith('.plist')
                || resource.endswith('.tmx')) {
                resources.push(resource);
            }
        }

        return resources;
    }

    function REResourceGetExtensionType (name) {
        if (name.endswith('.png')
            || name.endswith('.jpg')
            || name.endswith('.jpeg')
            || name.endswith('.gif')
            || name.endswith('.bmp')) {
            return 'image';
        } if (name.endswith('.plist')) {
            return 'plist';
        } if (name.startswith('bgm-')) {
            return 'bgm';
        } if (name.startswith('effect-')) {
            return 'effect';
        } else if (name.endswith('.tmx')) {
            return 'tmx';
        }
    }

    window_.REMapResourcesGetPath = function (aPath) {
        return REResourceGetPath(aPath);
    }
    
    window_.RERequestMapResources = function () {
        var preloadResources = [];

        /* ../Resoureces/iphone */
        /* root */
        jQuery.ajax({
            type: "post",
            async: false,
            url: "/services/listDirectory",
            data: {
                path: rootPath,
                folder: false
            },
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            cache: false,
            success: function (data) {
                var resources = REResourcesFilter(data),
                    resource,
                    item;

                for (var i = 0, len = resources.length; i < len; ++i) {
                    resource = resources[i];
                    item = {
                        type: REResourceGetExtensionType(resource),
                        src: REResourceGetMCResourcesPath(resource)
                    };
                    // console.log('{type: '+item.type+', src: '+item.src+'}');
                    preloadResources.push(item);
                }
            },
            error: function (err) {
                console.log(err);
            }
        });

        /* spritesheets */
        jQuery.ajax({
            type: "post",
            async: false,
            url: "/services/listDirectory",
            data: {
                path: spritesheetsPath,
                folder: false
            },
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            cache: false,
            success: function (data) {
                var resources = REResourcesFilter(data),
                    resource,
                    item;

                for (var i = 0, len = resources.length; i < len; ++i) {
                    resource = resources[i];
                    item = {
                        type: REResourceGetExtensionType(resource),
                        src: spritesheetsPath + '/' + resource
                    };
                    // console.log('{type: '+item.type+', src: '+item.src+'}');
                    preloadResources.push(item);
                }
            },
            error: function (err) {
                console.log(err);
            }
        });

        /* sounds */
        jQuery.ajax({
            type: "post",
            async: false,
            url: "/services/listDirectory",
            data: {
                path: soundsPath,
                folder: false,
                extension: '.mp3|.wav'
            },
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            cache: false,
            success: function (data) {
                var resources = REResourcesFilter(data),
                    resource,
                    item;

                for (var i = 0, len = resources.length; i < len; ++i) {
                    resource = resources[i];
                    item = {
                        type: REResourceGetExtensionType(resource),
                        src: soundsPath + '/' + resource
                    };
//                    console.log('{type: '+item.type+', src: '+item.src+'}');
                    preloadResources.push(item);
                }
            },
            error: function (err) {
                console.log(err);
            }
        });

        /* maps/ */
        jQuery.ajax({
            type: "post",
            async: false,
            url: "/services/listDirectory",
            data: {
                path: mapPath,
            },
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            cache: false,
            success: function (data) {
                var resources = REResourcesFilter(data),
                    resource,
                    item;

                for (var i = 0, len = resources.length; i < len; ++i) {
                    resource = resources[i];
                    item = {
                        type: REResourceGetExtensionType(resource),
                        src: mapPath + '/' + resource
                    };
                    preloadResources.push(item);
                }
            },
            error: function (err) {
                console.log(err);
            }
        });

        /* res/ */
        jQuery.ajax({
            type: "post",
            async: false,
            url: "/services/listDirectory",
            data: {
                path: 'res/',
            },
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            cache: false,
            success: function (data) {
                var resources = data,
                    resource;

                for (var i = 0, len = resources.length; i < len; ++i) {
                    resource = resources[i];
                    preloadResources.push({
                        type: REResourceGetExtensionType(resource),
                        src: REResourceGetPath(resource)
                    });
                }
            },
            error: function (err) {
                console.log(err);
            }
        });

        return preloadResources;
    };
})(window);
