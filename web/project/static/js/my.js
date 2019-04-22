var errorDialog = new mdui.Dialog('#errorDialog');
document.querySelector('#errorDialog').addEventListener('opened.mdui.dialog', function () {
    document.querySelector('#cancelBtn').focus()
    // document.querySelector('#errorMsg').textContent = document.activeElement.id
})

function openError(msg) {
    // mdui.alert(msg, 'ERROR');
    document.querySelector('#errorMsg').textContent = msg
    errorDialog.open()
}

var loginDialogue = new mdui.Dialog('#exampleDialog')


function openLogin() {
    loginDialogue.open()
}


var processDialogue = new mdui.Dialog('#processDialogue')

function eraseBtnClicked () {
    var nm = document.querySelector('#emailinput').value
    var ps = document.querySelector('#passwdinput').value

    // alert(location.href)

    // alert(nm + ps)
    loginDialogue.close()
    processDialogue.open()
    // document.querySelector('#baseDiv').setAttribute('class', 'disabled')
    var classVal = document.querySelector("body").getAttribute("class")
    document.querySelector("body").setAttribute('class', classVal + ' disabled')

    $.post('/login', {'user': nm, 'pswd': ps}, function (d) {
        // alert(d.status === '-1')
        processDialogue.close()
        document.querySelector("body").setAttribute('class', classVal)

        if (d.status === '-1') {
            openError('用户名或密码错误')
        } else {
            // document.execCommand('Refresh')
            var url = location.href.replace('#mdui-dialog','')
            // alert(url)
            location.href = url
        }
    })

    // window.navigate('/login')
}

function registerBtnClicked () {
    var nm = document.querySelector('#emailinputR').value
    var ps1 = document.querySelector('#passwdinputR1').value
    var ps2 = document.querySelector('#passwdinputR2').value

    if (ps1 !== ps2) {
        document.querySelector('#errorMsg').textContent = "两次输入密码不一致"
        errorDialog.open()
    }

    // $.post('/login', {'user': nm, 'pswd': ps}, function (d) {
    //     if (d.status == '-1') {
    //         alert('dismatch!')
    //     }
    //     location.reload()
    // })
    // window.navigate('/login')
}


var logoutbtn = document.querySelector('#logoutbtn')

if (logoutbtn) {
    logoutbtn.onclick = function () {
        $.post('/logout', {}, function (d) {
            location.reload()
        })
    }
}

function ClickUsername() {
    if (event.keyCode == 13) {
        var pswd = document.querySelector('#passwdinput')
        pswd.focus()
        event.returnValue = false
        return false
    }
}

function ClickPassword() {
    if (event.keyCode == 13) {
        // alert("enter pressed")
        eraseBtnClicked()
        event.returnValue = false
        return false
    }
}

var headicon = document.querySelector('#headicon')
if (headicon) {
    headicon.onclick = function () {
        location.href = "/account"
    }
}

var hitokoto = document.querySelector('#hitokoto')
if (hitokoto) {
    fetch('https://v1.hitokoto.cn')
        .then(function (res) {
            return res.json();
        })
        .then(function (data) {
            hitokoto.innerText = data.hitokoto;
            document.querySelector('#hitokotoSource').innerHTML = "—— " + data.from
        })
}

var data = [
    ['c100','2018-03-28 08:00','2018-03-28 08:23',[['一等座',2000,765.50],['二等座',2000,765.49],['三等座',2000,765.48]],'c','name1'],
    ['c100','2018-03-28 10:00','2018-03-28 10:23',[['一等座',2000,965.50],['二等座',2000,965.49],['三等座',2000,965.48]],'c','name2']
]

function createTable(){
    tableNode=document.querySelector('#tbody')
    var row = data.length;
    //上面确定了 现在开始创建

    for(var x=0;x<row;x++){
        var trNode=tableNode.insertRow();
        for(var y=-1;y<=3;y++){
            var tdNode=trNode.insertCell();
            if (y==-1) {
                tdNode.innerHTML = x + 1
            } else if (y==3) {
                var tx = "<div class='mdui-typo'>"
                var len = data[x][3].length
                for(var i=0;i<len;i++) {
                    tx += "<div>" + data[x][3][i][0] + " : 余" + data[x][3][i][1] + "张" + "</div>"
                }
                tdNode.innerHTML = tx
            } else {
                tdNode.innerHTML = data[x][y]
            }
        }
        var col = document.createElement('td')
        var btn = document.createElement('button')
        btn.setAttribute('class', 'mdui-btn mdui-ripple mdui-color-theme-accent')
        btn.innerText = '详情'
        // btn.setAttribute('mdui-dialog', "{target: '#detailDialogue'}")
        btn.setAttribute('onclick', '{clickedAtRow(' + x + ')}')
        col.append(btn)
        trNode.append(col)
    }
}

var tablePlace = document.querySelector('#tbody')
if (tablePlace) {
    createTable()
}

var detailDialog = new mdui.Dialog('#detailDialogue');

function clickedAtRow(x) {
    var tx = document.createElement('div')
    tx.setAttribute('class', 'mdui-typo')
    var len = data[x][3].length
    for(var i=0;i<len;i++) {
        tx.innerHTML += "<h1 class='mdui-text-color-theme' style='display: inline;'>" + data[x][3][i][0] + "</h1>"
        tx.innerHTML += "<p style='display: inline;'>" + ": 余" + data[x][3][i][1] + "张" + "</p> <br/>"
    }
    document.querySelector('#detailMsg').innerHTML = ""
    document.querySelector('#detailMsg').append(tx)


    detailDialog.open()
    document.querySelector('#detailCancleBtn').focus()
    // alert(row)
}