function filltable()
    {
        $(document).ready(function(){
                    
                $.ajax({
                    url: "cgi-bin/getfiles.py",// give your url
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
                                             + '<td>'+(i+1)+'</td>'
                                              +'<td>'+response[i]
                                            +'</tr>'  
                                
                        }
                        document.getElementById('tbl').innerHTML = innerhtml;
                        
                        
                    }
                });
            });
    }


    function addpattern()
    {
        var pattern = document.getElementById('pattern').value;
        var formData = new FormData();
        formData.append( 'pattern',pattern);
        $(document).ready(function(){
        
            $.ajax({
                url: "cgi-bin/insertfile.py",// give your url
                type: "POST",
                data: formData,
                dataType: 'json',
                processData: false,
                contentType: false,
                success: function (response) 
                {
                    document.getElementById('pattern').value = ''
                    filltable()
                }
            });
        });
    }

    filltable()