import requests
import json

url = 'https://www.pobanion.com'
#endpoint = '/api/test'
endpoint = '/api/plant/addtodb'
#endpoint = '/'
#url= http://pobanion.com/api/plant/addtodb
body = {"luminosity":"3002","temperature":"300"}

headers = {
        'content-type': 'application/json', 
        'Accept' : 'application/json',
        'user' : 'pobo', 
        'pass' : 'afsder', 
        'Connection' : 'Close'
        }
     
response = requests.post(url+endpoint, data=json.dumps(body), headers=headers)

#print (response.headers)
#print (response.status_code)
print (response.text)
response.close()



#response = requests.post(url+endpoint, data=json.dumps(body), headers=headers)
#req = requests.Request('POST', url+endpoint, headers=headers)
'''prepared = req.prepare()

def pretty_print_POST(req):
    """
    At this point it is completely built and ready
    to be fired; it is "prepared".

    However pay attention at the formatting used in 
    this function because it is programmed to be pretty 
    printed and may differ from the actual request.
    """
    print('{}\n{}\r\n{}\r\n\r\n{}'.format(
        '-----------START-----------',
        req.method + ' ' + req.url,
        '\r\n'.join('{}: {}'.format(k, v) for k, v in req.headers.items()),
        req.body,
    ))

pretty_print_POST(prepared)

s = requests.Session();
s.send(prepared)

print (s.headers)
#print ()
'''