(function($){
  $(function(){
    
 /* Settings
    =============================================== */
    // If no URL is hardcoded in the url variable then the user will be prompted
    // to enter a URL which will be stored in localStorage
    var url = '';
    
 /* Variables
    =============================================== */
  	var $icon      = $('.timepicker-icon');
  	var $timeInput = $('.timepicker');
    var $switch    = $('.switch').find('input[type=checkbox]');    
    var $loader    = $('.progress');
    var localStorageUrl = localStorage.getItem( 'url' );

 /* Functions
    =============================================== */
    function onInit() {
        setupListenEvents();

        getDeviceState();	    
    }

    function setupListenEvents() {
        $timeInput.timepicker({
            defaultTime: 'now',
            showClearBtn: true,
            vibrate: true
        });

        $timeInput.on('change', function() {
            onAlarmChange();        
        });

        // Launch timepicker on icon click
        $icon.on('click', function() {
            $timeInput.timepicker('open');
        });

        $switch.on("change", function() {
            var status = ($(this).prop('checked') ? 1 : 0);
            doAjaxRequest('POST', '/lamp', { lampOn : status } );
        });
    }   

    function getDeviceState() {
        // if no url is found then prompt user to enter url
        if (!url) {
            if ( localStorageUrl ) {
                url = localStorageUrl;
            } else {
                promptUserForUrl();
                return; 
            }
        }        
        
        var successCallback = function(data) {
            if ( data.alarmSet ) {
                setTime(data.alarmHour, data.alarmMin);
            }
            if ( data.lampOn ) {
                $switch.prop('checked', true);
            }            
            
            $('.main-container').fadeIn();            
        }

        $('.main-container').hide();
        $('.status-text').html('Connecting to device').show();        
        doAjaxRequest('GET', '/status', {}, successCallback);      
    }

    function onAlarmChange() {

        var alarmSet = ($timeInput.val() ? 1 : 0);

        var postData = {
            alarmSet: alarmSet
        };

        if ( alarmSet ) {
            // convert the time string into integers with 24 hour format
            var convertedTime = convertTimestring($timeInput.val());

            // add to post data
            postData.alarmHour = convertedTime.alarmHour;
            postData.alarmMin = convertedTime.alarmMin;
        }

        updateAlarmIcon();

        doAjaxRequest('POST', '/set', postData);

    }
		
    function doAjaxRequest(type, path, data, callback) {
        callback = (typeof callback !== 'undefined') ?  callback : false;
        data = (typeof data !== 'undefined') ?  data : {};
        
        $loader.show();

		$.ajax({ 
            type: type, 
            url: url + path,
            dataType: 'json',
            timeout: 8000,
            data: JSON.stringify(data),
            success: function(data) { 
                if ( typeof callback === 'function' ) {
                    callback(data);
                }              
                
                $('.status-text').hide();
                console.log(data);
            },
            error: function() { 
                
                $('.status-text').html('Error: no response from device');
                $('.status-text').show();

                // Prompt user to re-enter URL:
                setTimeout(function(){ promptUserForUrl(); }, 3000);
            },
            complete: function() {
                $loader.hide();
            }
        });
    }

    function promptUserForUrl() {
        var $modal = $('#modal-prompt').modal({
            dismissible: false,
            onCloseEnd: function() {

                setUrlInLocalStorage();

                // attempt to connect to device
                getDeviceState();
            }
        }); 
        $modal.modal('open');
    }

    function setUrlInLocalStorage() {
        // update global variable and store in local storage
        url = $( '#url' ).val();
        localStorage.setItem( 'url', url );        
    }

 /* Helper functions
    =============================================== */
    function setTime(hour, min) {
        var format = (hour >= 12 ? 'PM' : 'AM');
        hour = numberToString(hour);
        min = numberToString(min);
        $timeInput.val(hour+':'+min+' '+format);
        updateAlarmIcon();
    }

    function convertTimestring(timeString) {
        var alarmHour = parseInt(timeString.slice(0, 2));
        var alarmMin = parseInt(timeString.slice(3, 5));
        var format = timeString.slice(6, 8);

        if ( format == 'PM' ) {
            // convert to 24 hour time
            alarmHour = alarmHour + 12;
        }

        return { alarmHour:alarmHour, alarmMin: alarmMin };
    }
    
    // convert int to string and prefix number with 0 if required
    function numberToString(value) {
        value = value.toString();
        if ( value.length == 1 ) {
            value = '0' + value;
        }
        return value;
    }
    function updateAlarmIcon() {
        if ($timeInput.val() != '') {
                $icon.html('alarm_on').addClass('light-green-text lighten-1');
        } else {
            $timeInput.val('--:-- --');
            $icon.html('alarm_off').removeClass('light-green-text lighten-1');
        }
    }


 /* Let's go!
    =============================================== */
    onInit();    

  }); // end of document ready
})(jQuery); // end of jQuery name space
