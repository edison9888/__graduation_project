function REAppInit() {
    RENavigatorBarInit();
}

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

