(function (window_, $) {
    var kendo = window_.kendo,
        ui = kendo.ui,
        Widget = ui.Widget,
        FRONT = '.re-critical-hit-range-editor-front',
        SEPARATOR1 = '.re-critical-hit-range-editor-separator1',
        SIDE = '.re-critical-hit-range-editor-side',
        SEPARATOR2 = '.re-critical-hit-range-editor-separator2',
        BACK = '.re-critical-hit-range-editor-back',
        NS = '.reCriticalHitRangeEditor',
        INIT_DATA = 'init-data';

    function verify(aValue) {
        if (! /^([1-9]|1[0-9]|20)$|^(([1-9]|1[0-9]|20)\-([1-9]|1[0-9]|20))$/.test(aValue)) {
            return false;
        }

        var splits = aValue.split('-'),
            min = parseInt(splits[0]),
            max = splits.length == 2 ? parseInt(splits[1]) : min;

        return min <= max;
    }

    function onBlur() {
        if (! this.value) { /* 允许为空值 */
            $(this).css({
                color: 'red'
            });
            return;
        }
        if (! verify(this.value)) {
            $(this).css({
                color: 'red'
            }).focus().select();
        } else {
            $(this).css({
                color: 'white'
            });
        }
    }

    var RECriticalHitRangeEditor = Widget.extend({
        init: function(element, options) {
            var that = this;

            Widget.fn.init.call(that, element, options);

            options = that.options;

            if (!that.element.find(FRONT).length) {
                $('<input>').appendTo(that.element)
                            .addClass('k-textbox')
                            .addClass(FRONT.substring(1))
                            .attr({
                                maxlength: 5
                            })
                            .bind('blur', onBlur);
            }

            if (!that.element.find(SEPARATOR1).length) {
                that.element.append($('<span>')
                                    .html('/')
                                    .addClass(SEPARATOR1.substring(1)));
            }            

            if (!that.element.find(SIDE).length) {
                $('<input>').appendTo(that.element)
                            .addClass('k-textbox')
                            .addClass(SIDE.substring(1))
                            .attr({
                                maxlength: 5
                            })
                            .bind('blur', onBlur);
            }

            if (!that.element.find(SEPARATOR2).length) {
                that.element.append($('<span>')
                                    .html('/')
                                    .addClass(SEPARATOR2.substring(1)));
            }            

            if (!that.element.find(BACK).length) {
                $('<input>').appendTo(that.element)
                            .addClass('k-textbox')
                            .addClass(BACK.substring(1))
                            .attr({
                                maxlength: 5
                            })
                            .bind('blur', onBlur);
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
            name: 'CriticalHitRangeEditor'
        },

        loadData: function (aCriticalHitRangeString) {
            var splits = aCriticalHitRangeString.split('/'),
                f,
                s,
                b;

            if (splits.length != 3) {
                return undefined;
            }
            f = splits[0].trim();
            s = splits[1].trim();
            b = splits[2].trim();

            if (!verify(f) || !verify(s) || !verify(b)) {
                return undefined;
            }

            this.element.find(FRONT).val(f);
            this.element.find(SIDE).val(s);
            this.element.find(BACK).val(b);

            return this;
        },

        getData: function () {
            var f = this.element.find(FRONT).val(),
                s = this.element.find(SIDE).val(),
                b = this.element.find(BACK).val();

            if (!verify(f) || !verify(s) || !verify(b)) {
                return undefined;
            }

            return f + '/' + s + '/' + b;
        }
    });

    ui.plugin(RECriticalHitRangeEditor);
})(window, window.kendo.jQuery);
