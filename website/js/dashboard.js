    var linelbls = []
                        var linedt =  []
var ctx1 = document.getElementById('lineChart').getContext('2d');
    function line()
    {
                        
                        
                         $(document).ready(function(){
                                    
                                $.ajax({
                                    url: "cgi-bin/top5processes.py",// give your url
                                    type: "POST",
                                    dataType: 'json',
                                    processData: false,
                                    contentType: false,
                                    success: function (response) 
                                    {
                                        
                                        for(var i=0;i<response.length;i++)
                                        {
                                            linelbls.push(response[i].pid);
                                            linedt.push(response[i].cnt);                
                                                
                                        }
                                       
                                        
                                    }
                                });
                            });
                            setTimeout("drawline()", 2000);
                          

    }

    function drawline()
    {
        var myChart = new Chart(ctx1, {
                                            type: 'line',
                                            data: {
                                                labels: linelbls,
                                                datasets: [{
                                                    label: 'TOP 5 PROCESSES',
                                                    data: linedt,
                                                    backgroundColor: [
                                                        'rgba(255, 99, 132, 1)',
                                                        'rgba(54, 162, 235, 1)',
                                                        'rgba(255, 206, 86, 1)',
                                                        'rgba(75, 192, 192, 1)',
                                                    ],
                                                    borderColor: [
                                                        'rgba(255,99,132,1)',
                                                        'rgba(54, 162, 235, 1)',
                                                        'rgba(255, 206, 86, 1)',
                                                        'rgba(75, 192, 192, 1)',
                                                    ],
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



    var pielbls = []
        var piedt =  []
     var ctx2 = document.getElementById('doughChart').getContext('2d');
    function pie()
    {

       
         $(document).ready(function(){
                    
                $.ajax({
                    url: "cgi-bin/top5systemcalls.py",// give your url
                    type: "POST",
                    dataType: 'json',
                    processData: false,
                    contentType: false,
                    success: function (response) 
                    {
                        
                        for(var i=0;i<response.length;i++)
                        {
                            pielbls.push(response[i][0]);
                            piedt.push(response[i][1]);                
                                
                        }
                        
                        
                    }
                });
            });

         setTimeout("drawpie()", 2000);
          

    }


    function drawpie()
    {
        var myChart = new Chart(ctx2, {
                                            type: 'doughnut',
                                            data: {
                                                labels: pielbls,
                                                datasets: [{
                                                    label: 'TOP 5 SYSTEM CALLS',
                                                    data: piedt,
                                                    backgroundColor: [
                                                        'rgba(54, 162, 235, 1)',
                                                        'rgba(255, 206, 86, 1)',
                                                        'rgba(75, 192, 192, 1)',
                                                        'rgba(153, 102, 255, 1)',
                                                        'rgba(255, 159, 64, 1)'
                                                    ],
                                                    borderColor: [
                                                        
                                                        'rgba(54, 162, 235, 1)',
                                                        'rgba(255, 206, 86, 1)',
                                                        'rgba(75, 192, 192, 1)',
                                                        'rgba(153, 102, 255, 1)',
                                                        'rgba(255, 159, 64, 1)'
                                                    ],
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
                                                },
                                                
                                            }
                                        }); 
    }

    function filltable()
    {
        $(document).ready(function(){
                    
                $.ajax({
                    url: "cgi-bin/top5pidswithtop5systemcalls.py",// give your url
                    type: "POST",
                    dataType: 'json',
                    processData: false,
                    contentType: false,
                    success: function (response) 
                    {
                        var innerhtml = ""
                        for(var i=0;i<response.length;i++)
                        {
                                         
                                innerhtml += ' <tr>'
                                             + '<td>'+response[i][0]+'</td>'
                                              +'<td>'+response[i][1]+'</td>'
                                              +'<td>'+response[i][2]+'</td>'
                                            +'</tr>'  
                                
                        }
                        document.getElementById('tbl').innerHTML = innerhtml;
                        
                        
                    }
                });
            });
    }

    /*
    function area()
    {

     var lbls = []
        var dt =  []
        $(document).ready(function(){
                    
                $.ajax({
                    url: "cgi-bin/top5systemcalls.py",// give your url
                    type: "POST",
                    dataType: 'json',
                    processData: false,
                    contentType: false,
                    success: function (response) 
                    {
                        
                        for(var i=0;i<response.length;i++)
                        {
                            lbls.push(response[i][0]);
                            dt.push(response[i][1]);                
                                
                        }
                        alert(lbls);
                        
                    }
                });
            });

         var data = google.visualization.arrayToDataTable(dt);
         var options = {
              title: 'Company Performance',
              hAxis: {title: 'Year',  titleTextStyle: {color: '#333'}},
              vAxis: {minValue: 0}
            };

            var chart = new google.visualization.AreaChart(document.getElementById('chart_div'));
            chart.draw(data, options);
    }
    */

    line()
    pie()
    filltable()
