
var url = new URL(window.location.href);
var pid = url.searchParams.get("pid");
document.getElementById('h').innerHTML = 'TRACE OF PROCESS ID : '+pid
function readTextFile(file)
{
    var rawFile = new XMLHttpRequest();
    file = "logs/dirlist_"+file+".txt";
    rawFile.open("GET", file, false);
    rawFile.onreadystatechange = function ()
    {
        if(rawFile.readyState === 4)
        {
            if(rawFile.status === 200 || rawFile.status == 0)
            {
                var allText = rawFile.responseText;
                document.getElementById('strace').innerHTML = allText;
                
            }
        }
    }
    rawFile.send(null);
}

var linelbls = []
var linedt =  []
var formData = new FormData();
formData.append( 'pid',pid);
    
 $(document).ready(function(){
        
        $.ajax({
            url: "cgi-bin/getprocessstrace.py",// give your url
            type: "POST",
            data: formData,
            dataType: 'json',
            processData: false,
            contentType: false,
            success: function (response) 
            {
                for(var i=0;i<response.length;i++)
                {
                    linelbls.push(response[i][0]);
                    linedt.push(response[i][1]);                
                        
                }
                 setTimeout("drawline()", 2000);
            }
        });
    });


function drawline()
{
    var ctx = document.getElementById('linechart').getContext('2d');
    var myChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: linelbls,
            datasets: [{
                label: 'Top System Calls Made',
                data: linedt,
                backgroundColor: 'rgba(0,0,204,0.7)',
                borderWidth: 1
            }]
        },
        options: {
            scales: {
                yAxes: [{
                    ticks: {
                        beginAtZero:true
                    }
                }]
            }
        }
    });

}


readTextFile(pid)