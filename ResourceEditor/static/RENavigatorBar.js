(function (window_) {
    var kRENavigatorBarId = 're-navigator-bar';
    var kRENavigatorBarJQuery = '#re-navigator-bar';

    function REURLChangeTo(path) {
        window_.location.href = path;
    }

    function RENavigatorBarItemCreate(aHTML, anURL)
    {
        return $('<button>').html(aHTML)
                            .addClass('k-button')
                            .addClass('navigator-bar-button')
                            .bind('click', function () {
                               REURLChangeTo(anURL);
                            });
    }

    window_.RENavigatorBarInit = function () {
        var navigatorBar = $('<div>').attr({
                                        id: kRENavigatorBarId
                                        }).appendTo(document.getElementById('navigator-bar'))
                                          .addClass('navigator-bar');

        RENavigatorBarItemCreate('首页', 'index.html').appendTo(navigatorBar);
        RENavigatorBarItemCreate('NPC编辑器', 'npcs.html').appendTo(navigatorBar);
        RENavigatorBarItemCreate('任务编辑器', 'tasks.html').appendTo(navigatorBar);
        RENavigatorBarItemCreate('武器编辑器', 'weapons.html').appendTo(navigatorBar);
        RENavigatorBarItemCreate('技能编辑器', 'skills.html').appendTo(navigatorBar);
        RENavigatorBarItemCreate('防具编辑器', 'armors.html').appendTo(navigatorBar);
        RENavigatorBarItemCreate('道具编辑器', 'props.html').appendTo(navigatorBar);
        RENavigatorBarItemCreate('矿石编辑器', 'ores.html').appendTo(navigatorBar);
        RENavigatorBarItemCreate('怪物编辑器', 'monsters.html').appendTo(navigatorBar);
        RENavigatorBarItemCreate('场景编辑器', 'scenes.html').appendTo(navigatorBar);
   };
})(window);
