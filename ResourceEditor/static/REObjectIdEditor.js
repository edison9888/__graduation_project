(function (window_, $) {
    var kendo = window_.kendo,
        ui = kendo.ui,
        Widget = ui.Widget,
        CLASS = '.re-objid-editor-class',
        SUBCLASS = '.re-objid-editor-sub-class',
        INDEX = '.re-objid-editor-index',
        SUBINDEX = '.re-objid-editor-sub-index',
        NS = '.reObjectIdEditor',
        INIT_DATA = 'init-data';

    function verifyClass(char) {
        if (char >= 'A' && char <= 'Z') {
            return true;
        }

        return false;
    }

    function verifySubClass(char) {
        if (char >= '0' && char <= '9'
            || char >= 'A' && char <= 'Z') {
            return true;
        }

        return false;
    }

    function verifyIndex(char) {
        if (char >= '0' && char <= '9'
            || char >= 'A' && char <= 'Z') {
            return true;
        }

        return false;
    }

    function verifySubIndex(char) {
        if (char >= '0' && char <= '9') {
            return true;
        }

        return false;
    }

    function onBlur(anElement, aVerifyCallback) {
        if (! anElement.value) { /* 允许为空值 */
            $(anElement).css({
                color: 'red'
            });
            return;
        }
        if (! aVerifyCallback(anElement.value)) {
            $(anElement).css({
                color: 'red'
            }).focus().select();
        } else {
            $(anElement).css({
                color: 'white'
            });
        }
    }

    var REObjectIdEditor = Widget.extend({
        init: function(element, options) {
            var that = this;

            Widget.fn.init.call(that, element, options);

            options = that.options;
            if (!that.element.find(CLASS).length) {
                that.element.append($('<input>').attr({
                                        maxlength: 1
                                    })
                                    .addClass('k-textbox')
                                    .addClass(CLASS.substring(1))
                                    .bind('blur', function (e) {
                                        onBlur(this, verifyClass);
                                    }));
            }

            if (!that.element.find(SUBCLASS).length) {
                that.element.append($('<input>').attr({
                                        maxlength: 1
                                    })
                                    .addClass('k-textbox')
                                    .addClass(SUBCLASS.substring(1))
                                    .bind('blur', function (e) {
                                        onBlur(this, verifySubClass);
                                    }));
            }

            if (!that.element.find(INDEX).length) {
                that.element.append($('<input>').attr({
                                        maxlength: 1
                                    })
                                    .addClass('k-textbox')
                                    .addClass(INDEX.substring(1))
                                    .bind('blur', function (e) {
                                        onBlur(this, verifyIndex);
                                    }));
            }

            if (!that.element.find(SUBINDEX).length) {
                that.element.append($('<input>').attr({
                                        maxlength: 1
                                    })
                                    .addClass('k-textbox')
                                    .addClass(SUBINDEX.substring(1))
                                    .bind('blur', function (e) {
                                        onBlur(this, verifySubIndex);
                                    }));
            }

            if (that.element.attr(INIT_DATA)) {
                that.loadData(that.element.attr(INIT_DATA));
            }
        },

        destroy: function () {
            var that = this;

            Widget.fn.destroy.call(that);

            that.element.off(NS);
        },

        events: [],

        options: {
            name: 'ObjectIdEditor'
        },

        loadData: function (anObjectIdString) {
            if (anObjectIdString.length < 4) {
                return undefined;
            }
            this.element.find(CLASS).val(anObjectIdString[0]);
            this.element.find(SUBCLASS).val(anObjectIdString[1]);
            this.element.find(INDEX).val(anObjectIdString[2]);
            this.element.find(SUBINDEX).val(anObjectIdString[3]);

            return this;
        },

        getData: function () {
            var class_ = this.element.find(CLASS).val(),
                subClass_ = this.element.find(SUBCLASS).val(),
                index_ = this.element.find(INDEX).val(),
                subIndex_ = this.element.find(SUBINDEX).val();

            if (!class_ || !subClass_ || !index_ || !subIndex_) {
                return null;
            }

            return class_ + subClass_ + index_ + subIndex_;
        }
    });

    ui.plugin(REObjectIdEditor);
})(window, window.kendo.jQuery);
