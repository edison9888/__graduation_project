(function (window_, $) {
    var kendo = window_.kendo,
        ui = kendo.ui,
        Widget = ui.Widget,
        COUNT = '.re-dice-editor-count',
        DICE = '.re-dice-editor-dice',
        VALUE = '.re-dice-editor-value',
        NS = '.reDiceEditor',
        INIT_DATA = 'init-data';
        window_.countDataSource = [
            { text: '1', value: 1, index: 0 },
            { text: '2', value: 2, index: 1 },
            { text: '3', value: 3, index: 2 },
            { text: '4', value: 4, index: 3 }
        ],
        window_.valueDataSource = [
            { text: '4', value: 4, index: 0 },
            { text: '6', value: 6, index: 1 },
            { text: '8', value: 8, index: 2 },
            { text: '10', value: 10, index: 3 },
            { text: '12', value: 12, index: 4 },
            { text: '20', value: 20, index: 5 }
        ];

    function verify(aValue) {
        return /^[1-4][dD]4|6|8|10|12|20$/.test(aValue);
    }

    var REDiceEditor = Widget.extend({
        init: function(element, options) {
            var that = this;

            Widget.fn.init.call(that, element, options);

            options = that.options;

            if (!that.element.find(COUNT).length) {
                $('<input>').appendTo(that.element)
                            .kendoDropDownList({
                                dataTextField: 'text',
                                dataValueField: 'value',
                                dataSource: countDataSource
                            });
                that.element.find('span:nth-child(1)[role]').addClass(COUNT.substring(1));
            }

            if (!that.element.find(DICE).length) {
                that.element.append($('<span>')
                                    .html('D')
                                    .addClass(DICE.substring(1)));
            }

            if (!that.element.find(VALUE).length) {
                $('<input>').appendTo(that.element)
                            .kendoDropDownList({
                                dataTextField: 'text',
                                dataValueField: 'value',
                                dataSource: valueDataSource
                            });
                that.element.find('span:nth-child(3)[role]').addClass(VALUE.substring(1));
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
            name: 'DiceEditor'
        },

        loadData: function (aValue) {
            if (! verify(aValue)) {
                return null;
            }

            var count_ = parseInt(aValue[0]),
                value_ = parseInt(aValue.substring(2));
            
            $(this.element.find('input')[0])
                          .data('kendoDropDownList')
                          .select(countDataSource.filter(function (data) {
                              return data.value === count_;
                          })[0].index);
            $(this.element.find('input')[1])
                        .data('kendoDropDownList')
                        .select(valueDataSource.filter(function (data) {
                            return data.value === value_;
                        })[0].index);
            
            return this;
        },

        getData: function () {
            var count_ = this.element.find(COUNT + ' span span[class="k-input"]').html(),
                value_ = this.element.find(VALUE + ' span span[class="k-input"]').html();

            return count_ + 'D' + value_;
        }
    });

    ui.plugin(REDiceEditor);
})(window, window.kendo.jQuery);
