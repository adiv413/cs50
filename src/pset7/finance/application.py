from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash
from passlib.apps import custom_app_context as password
from helpers import apology, login_required, lookup, usd
#import urllib.requests

# Configure application
app = Flask(__name__)
# Ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    stock_portfolio = db.execute("SELECT price, symbol, shares FROM portfolio WHERE id = :id", id = session["user_id"])
    totalcash = 0
    updated_portfolio = 0
    for portsymbol in stock_portfolio:
        symbol = portsymbol["Symbol"]
        prices = portsymbol["Price"]
        shares = portsymbol["Shares"]
        shares1 = shares
        stock = lookup(symbol)
        stock_cash = shares * stock["price"]
        totalcash += stock_cash
        if prices != None:
            try:
                db.execute("UPDATE portfolio SET price = :price WHERE id = :id AND symbol = :symbol, price = usd(stock['price']), id = session['user_id'], symbol = symbol")
            except:
                db.execute("INSERT INTO portfolio (name, shares, price, total, symbol, id) VALUES(:name, :shares, :price, :total, :symbol, :id)", name=stock["name"], shares=shares, price=usd(stock["price"]), total=usd(shares * stock["price"]), symbol=stock["symbol"], id=session["user_id"])
     # update user's cash in portfolio
            updated_portfolio = db.execute("SELECT * FROM portfolio WHERE id = :id", id = session["user_id"])
    # update total cash -> cash + shares worth
            a = updated_portfolio[0]["TOTAL"]
            totalcash += float(a[1:].replace(",", ""))
        else:
            updated_portfolio = db.execute("SELECT * FROM portfolio WHERE id = :id", id = session["user_id"])
    updated_cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
    # print portfolio in index homepage
    return render_template("index.html", stocks=str(updated_portfolio), cash=usd(updated_cash[0]["cash"]), total = usd(totalcash))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        uisymbol = request.form.get("symbol")
        uishares = request.form.get("shares")
        db.execute("UPDATE portfolio SET shares = :shares WHERE id = :id AND symbol = :symbol", shares = uishares, id = session["user_id"], symbol = uisymbol)
        stock_symbol = lookup(uisymbol)
        db.execute("UPDATE portfolio SET price = :price WHERE id = :id", price = stock_symbol["price"], id = session["user_id"])
        stock_portfolio = db.execute("SELECT symbol, price, shares, id FROM portfolio WHERE id = :id", id = session["user_id"])
        try:
            shares = int(request.form.get("shares"))
            if shares < 1:
                return apology("Number of shares must be above one")
        except:
            return apology("Number of shares must be above one")
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        if not cash[0] or float(cash[0]['cash']) < shares * stock_symbol["price"]:
            return apology("Not enough money")
        db.execute("INSERT INTO history (symbol, shares, price, id) VALUES(:symbol, :shares, :price, :id)", symbol=stock_symbol["symbol"], shares=shares, price=usd(stock_symbol["price"]), id=session["user_id"])
        db.execute("UPDATE users SET cash = cash - :purchase WHERE id = :id", id=session["user_id"], purchase=stock_symbol["price"] * float(shares))
        user_shares = db.execute("SELECT shares FROM portfolio WHERE symbol = :symbol AND id = :id", symbol = stock_symbol["symbol"], id = session["user_id"])
        if not user_shares:
            db.execute("INSERT INTO portfolio (name, shares, price, total, symbol, id) VALUES(:name, :shares, :price, :total, :symbol, :id)", name=stock_symbol["name"], shares=shares, price=usd(stock_symbol["price"]), total=usd(shares * stock_symbol["price"]), symbol=stock_symbol["symbol"], id=session["user_id"])
        else:
            shares1 = user_shares[0]["Shares"]
            shares_total = shares1 + shares
            db.execute("UPDATE portfolio SET shares=:shares WHERE id=:id AND symbol=:symbol", shares=shares_total, id=session["user_id"], symbol=stock_symbol["symbol"])
        return redirect(url_for("index"))


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * from history WHERE id=:id", id=session["user_id"])

    return render_template("history.html", history=history)



@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""
    # Forget any user_id
    session.clear()
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        checkpwd = None
        try:
            checkpwd = check_password_hash(rows[0]["hash"], request.form.get("password"))
        except TypeError:
            pass
        # Ensure username exists and password is correct
        if len(rows) != 1 or checkpwd:
            return apology("invalid username and/or password", 403)


        # Redirect user to register page
            return redirect(url_for("register"))
        else:
            session["user_id"] = rows[0]["id"]
            return render_template("index.html")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        rows = lookup(request.form.get("symbol"))

        if not rows:
            return apology("Invalid Symbol")

        return render_template("quoted.html", stock=rows)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":

    # ensure username was submitted
        if not request.form.get("username"):
            return apology("Must provide username")
    # ensure password was submitted
        elif not request.form.get("password"):
            return apology("Must provide password")
    # ensure password and verified password is the same
        elif request.form.get("password") != request.form.get("passwordagain"):
            return apology("Passwords must match")
    # insert the new user into users, storing the hash of the user's password
        newuser = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username = request.form.get("username"), hash = password.hash(request.form.get("password")))
        if not newuser:
            return apology("Username already exists")
    # remember which user has logged in\
        session["user_id"] = newuser
    # redirect user to home page
        return redirect(url_for("index"))
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        return render_template("sell.html")
    else:
        # ensure proper symbol
        stock = lookup(request.form.get("symbol"))
        if not stock:
            return apology("Invalid Symbol")

        # ensure proper number of shares
        try:
            shares = int(request.form.get("shares"))
            if shares < 0:
                return apology("Shares must be positive integer")
        except:
            return apology("Shares must be positive integer")

        # select the symbol shares of that user
        user_shares = db.execute("SELECT shares FROM portfolio \
                                 WHERE id = :id AND symbol=:symbol", \
                                 id=session["user_id"], symbol=stock["symbol"])

        # check if enough shares to sell
        if not user_shares or int(user_shares[0]["Shares"]) < shares:
            return apology("Not enough shares")

        # update history of a sell
        db.execute("INSERT INTO history (symbol, shares, price, id) \
                    VALUES(:symbol, :shares, :price, :id)", \
                    symbol=stock["symbol"], shares=-shares, \
                    price=usd(stock["price"]), id=session["user_id"])

        # update user cash (increase)
        db.execute("UPDATE users SET cash = cash + :purchase WHERE id = :id", \
                    id=session["user_id"], \
                    purchase=stock["price"] * float(shares))

        # decrement the shares count
        shares_total = user_shares[0]["Shares"] - shares

        # if after decrement is zero, delete shares from portfolio
        if shares_total == 0:
            db.execute("DELETE FROM portfolio \
                        WHERE id=:id AND symbol=:symbol", \
                        id=session["user_id"], \
                        symbol=stock["symbol"])
        # otherwise, update portfolio shares count
        else:
            db.execute("UPDATE portfolio SET shares=:shares \
                    WHERE id=:id AND symbol=:symbol", \
                    shares=shares_total, id=session["user_id"], \
                    symbol=stock["symbol"])

        # return to index
        return redirect(url_for("index"))


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
