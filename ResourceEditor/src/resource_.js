
// var s_metaTiles = "res/meta_tiles.png";
// var s_TileA = "res/vx_map/TileA2.png";
// var s_isoTest = "res/iso-test.png";
// var s_isoTest2 = "res/iso-test2.png";

// var s_TMXMap1 = "res/1.tmx";
// var s_TMXMap2 = "res/2.tmx";
// var s_isoTest2TMX = "res/iso-test2.tmx";

// var s_orthogonalTest1 = "res/ortho-test1.png";
// var s_orthogonalTest1BW = "res/ortho-test1_bw.png";
// var s_orthogonalTest1TMX = "res/orthogonal-test1.tmx";

// var s_ortho_test2_png = "res/fixed-ortho-test2.png";
// var s_orthogonalTest2TMX = "res/orthogonal-test2.tmx";

// var s_tmw_desert_spacing_png = "res/tmw_desert_spacing.png";
// var s_tmw_desert_spacing_hd_png = "res/tmw_desert_spacing-hd.png";
// var s_orthogonalTest4TMX = "res/orthogonal-test4.tmx";

// var g_ressources = [
//     //image
//     {type:"image", src:s_metaTiles},
//     {type:"image", src:s_TileA},
//     {type:"image", src:s_isoTest},
//     {type:"image", src:s_isoTest2},
//     {type:"image", src:s_orthogonalTest1},
//     {type:"image", src:s_orthogonalTest1BW},
//     {type:"image", src:s_tmw_desert_spacing_png},
//     {type:"image", src:s_tmw_desert_spacing_hd_png},
//     {type:"image", src:s_ortho_test2_png},

//     //plist

//     //fnt

//     //tmx
//     {type:"tmx", src:s_TMXMap1},
//     {type:"tmx", src:s_TMXMap2},
//     {type:"tmx", src:s_isoTest2TMX},
//     {type:"tmx", src:s_orthogonalTest1TMX},
//     {type:"tmx", src:s_orthogonalTest2TMX},
//     {type:"tmx", src:s_orthogonalTest4TMX}

//     //bgm

//     //effect
// ];

(function (window_) {
	var dataPath = REResourceGetPath('maps'),
		realDataPath = REResourceGetRealPath('maps');

	String.prototype.endswith = function (s) { 
	    var i = this.indexOf(s), 
	        tl = this.length, 
	        sl = s.length; 

	    return tl - i == sl; 
	}

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
		} else if (name.endswith('.tmx')) {
			return 'tmx';
		}
	}

	window_.REMapResourcesGetPath = function (aPath) {
		return REResourceGetPath(aPath);
	}
        
	window_.RERequestMapResources = function () {
		var preloadResources = [];

		jQuery.ajax({
            type: "post",
            async: false,
            url: "/services/listDirectory",
            data: {
                path: dataPath,
                linkto: realDataPath
            },
            contentType: "application/json; charset=utf-8",
            dataType: "json",
            cache: false,
            success: function (data) {
				var resources = REResourcesFilter(data),
					resource;

				for (var i = 0, len = resources.length; i < len; ++i) {
					resource = resources[i];
					preloadResources.push({
						type: REResourceGetExtensionType(resource),
						src: REResourceGetPath('maps/' + resource)
					});
				}
			},
			error: function (err) {
                alert(err);
            }
        });

		console.log(preloadResources)
		return preloadResources;
	};
})(window);
