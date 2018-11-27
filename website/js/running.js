

function getpids()
{
    var colors=['success','warning','info']
    $(document).ready(function(){
            
        $.ajax({
            url: "cgi-bin/getrunningpids.py",// give your url
            type: "POST",
            dataType: 'json',
            processData: false,
            contentType: false,
            success: function (response) 
            {
                var innerhtml = ''
                for(var i=0;i<response.length-1;i++)
                {
                    innerhtml += '<div class="alert alert-'+colors[i%colors.length]+' col-sm-1 " style="cursor: pointer;margin:5px" onclick="window.open(\'pid.html?pid='+response[i]+'\')">'
                                 + '<p class="blink">'+response[i]+'</p>'
                                +'</div>';
                                
                    
                }
                document.getElementById('pidtable').innerHTML = innerhtml;
            }
        });
    });
}

setInterval(function(){ getpids(); }, 1000);
