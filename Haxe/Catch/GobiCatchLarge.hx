// Second Haxe game for Flash 8 for Wii
// Move mouse, Gobi Y coords follows mouse, pickup stuff

class GobiCatchLarge {
	static function main() {
		var Score:Int = 0;
		var TimeLimit:Int = (30*30);
		var Timer:Int = 0;

		var text:flash.TextField;
		var textform = new flash.TextFormat();
		text = flash.Lib.current.createTextField("scoretext",0,200,0,200,19);
		textform.size = 16;
		text.setNewTextFormat(textform);
		text._x = 200;
		text.text = "Score: " + Score;

		var gobi = flash.Lib.current.attachMovie("gobi", "gobi1", 1);
		gobi._x = 5;
		gobi._y = 100;

		var shark = flash.Lib.current.attachMovie("shark", "gh1",3);
		shark._x = 720;
		shark._y = 200;

		var coin = flash.Lib.current.attachMovie("coin", 's1',2);
		coin._x = 770;
		coin._y = 100;

		flash.Mouse.hide();

		flash.Lib.current.onEnterFrame = function() {
			if (Timer >= TimeLimit) {
				text.text = "Game Over! Your Score Is: " + Score;
				gobi._visible = false;
				shark._visible = false;
				coin._visible = false;
			}

			else {
				Timer += 1;
				shark._x -= 10;
				coin._x -= 5;
				if (shark.hitTest(gobi)) {
					shark._x = 720;
					shark._y = Std.random(260);
					Score -= 10;
				}
				else if (shark._x <= -80) {
					shark._x = 720;
					shark._y = Std.random(260);
				}
				if (coin.hitTest(gobi)) {
					coin._x = 770;
					coin._y = Std.random(270);
					Score += 5;
				}
				else if (coin._x <= -30) {
					coin._x = 770;
					coin._y = Std.random(270);
				}
				text.text = "Score: " + Score;
			}
		}

		flash.Lib.current.onMouseMove = function() {
			gobi._y = flash.Lib.current._ymouse;
		}

	}
}
