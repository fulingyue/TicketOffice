from flask import Flask
from flask import request, redirect
from flask import render_template
from flask import session, make_response, jsonify
from datetime import timedelta
from random import randint
import interaction
import sys

app = Flask("TTRS")
app.config['SEND_FILE_MAX_AGE_DEFAULT'] = timedelta(seconds=1)
app.jinja_env.auto_reload = True
app.secret_key = "19260817"
app.permanent_session_lifetime = timedelta(seconds=60*30)

con = interaction.intereaction()

@app.route('/')
def hello():
    if 'user' not in session:
        return render_template('index.html', ses = session)
    else:
        return render_template('index.html', ses = session)


@app.route('/login', methods=['POST'])
def login():
    if request.method == 'POST':
        user = request.form['user']
        pswd = request.form['pswd']
        print('NAME IS ' + user)
        print(request, request.form)

        res = con.exeCmd(user)

        print("RECEIVED " + res)

        if res == '1sd' :
            session['user'] = user
            session['headIcon'] = randint(1, 15)
            print(session)
            session.permanent = True
            return jsonify({'status': '0'})
        else :
            return jsonify({'status': '-1'})


@app.route('/logout', methods=['POST'])
def logout():
    if request.method == 'POST':
        del session['user']
        session.permanent = True
        return jsonify({'statsu': '0'})

@app.route('/account')
def account():
    return render_template('account.html', ses=session)

@app.route('/query')
def query():
    return render_template('query.html', ses=session)

@app.route('/register')
def reg():
    pass

@app.route('/manage')
def manage():
    return render_template('mdui.html', ses=session)

@app.route('/sendcmd', methods=['POST'])
def sendcmd():
    if request.method == 'POST':
        if 'key' in request.form:
            k = int(request.form['key'])
            if k == 12345678:
                con.exeCmd(request.form['cmd'])


if __name__ == '__main__':
    con.init()
    if len(sys.argv) == 3:
        app.run(host=str(sys.argv[1]), port=int(sys.argv[2]))
    else:
        app.run(port=9999)