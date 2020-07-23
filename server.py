from flask import Flask, request
from flask import jsonify
import pandas as pd

app = Flask(__name__)
"""
curl -X POST -H "Content-Type: application/json" -d '{"tmp" : 30, "h" : "50", "t":"test"}' "http://127.0.0.1:6000/post_json"
"""
@app.route("/post_json", methods=['POST'])
def post_json():
    r = request.get_json()
    
    try:
        data = [r["t"],r["tmp"],r["h"]]
        
        df = pd.DataFrame([data])
        df.to_csv('data.csv', mode='a+', header=False, index=None)
    except:
        return 'error'
    return jsonify(r)

if __name__ == '__main__':
    app.debug = True
    #app.run()
    app.run(host='0.0.0.0', port=6000)
    