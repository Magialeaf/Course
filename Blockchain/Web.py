from flask import Flask, render_template

app = Flask(__name__)

@app.route('/')
def index_main():
    return render_template('site.html')

@app.errorhandler(500)
def page_error(error):
    return "page not found"

if __name__ == '__main__':
    app.run(debug=True,host='0.0.0.0',port=5000)
