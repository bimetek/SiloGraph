var map;
var markers = {};
var markerIndex = 0;

function initialize(east, west, south, north)
{
    var mapOptions = {
        "center": new google.maps.LatLng(north, east),
        "mapTypeId": google.maps.MapTypeId.ROADMAP,
        "mapTypeControl": false,
        "streetViewControl": false,
        "zoomControlOptions": {"style": "small"},
        "keyboardShortcuts": false,
        "zoom": 9
    };
    map = new google.maps.Map(document.getElementById("map_canvas"),
                              mapOptions);
    var sw = new google.maps.LatLng(south, west);
    var ne = new google.maps.LatLng(north, east);
    map.fitBounds(new google.maps.LatLngBounds(sw, ne));
}

function appendMarker(name, latitude, longitude)
{
    var marker = new google.maps.Marker({
        position: new google.maps.LatLng(latitude, longitude),
        map: map,
        title: name,
        animation: google.maps.Animation.DROP
    });

     google.maps.event.addListener(marker, "click", function() {
         var index = -1;
         for (var key in markers)
         {
             if (markers[key] === marker)
             {
                 index = key;
                 break;
             }
         }
         qMapView.onMarkerClicked(index);
     });

    markers[markerIndex] = marker;
    markerIndex++;

    return markerIndex - 1;
}
