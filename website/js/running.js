function readTextFile(file)
{
    var rawFile = new XMLHttpRequest();
    file = "./logs/dirlist_"+file+".txt";
    rawFile.open("GET", file, false);
    rawFile.onreadystatechange = function ()
    {
        if(rawFile.readyState === 4)
        {
            if(rawFile.status === 200 || rawFile.status == 0)
            {
                var allText = rawFile.responseText;
                alert(allText);
            }
        }
    }
    rawFile.send(null);
}

function getpids()
{
    var colors=['success','warning','info']
    $(document).ready(function(){
            
        $.ajax({
            url: "cgi-bin/getrunningpids.py.py",// give your url
            type: "POST",
            dataType: 'json',
            processData: false,
            contentType: false,
            success: function (response) 
            {
                var innerhtml = ''
                for(var i=0;i<response.length-1;i++)
                {
                    innerhtml += '<div class="alert alert-'+colors[i%colors.length]+' col-sm-1 " style="cursor: pointer;margin:5px" onclick="readTextFile('+response[i]+')">'
                                 + '<p >'+response[i]+'</p>'
                                +'</div>';
                                
                    
                }
                document.getElementById('pidtable').innerHTML = innerhtml;
            }
        });
    });
}

setInterval(function(){ getpids(); }, 1000);
