(function (window_) {
    var __resPath = 'res';
    var __fakePath = 'fakepath'
    var __resourcePath = '../Resources/iphone';

    window_.REFakepathToRealPath = function (aFakePath) {
        return aFakePath.replace(__fakePath + '/', __resourcePath + '/')
    }

    window_.REResourceGetPath = function (aPath) {
        return __resPath + '/' + aPath;
    };

    window_.REResourceGetMCResourcesPath = function (aPath) {
        return __fakePath + '/' + aPath;
    };

    window_.REMakeEnum = function (offset) {
        return 1 << offset;
    }

    window_.REDictGetLength = function (dict) {
        var count = 0;

        for (var _ in dict) {
            ++count;
        }

        return count;
    }
    
    window_.puts = function (o) {
        console.log(o);
    };
})(window);
