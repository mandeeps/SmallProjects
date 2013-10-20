// Haxe port of GobiShoe to Flash8 for Wii

class GobiShoe {
	static function main() {
		var GobiDir:String = 'left';
		var Score:Int = 0;
		var Hit:Bool = false;
		var GobiSpeed:Int = 10;
		var HitTimer:Int = 0;

		var text:flash.TextField;
		var textform = new flash.TextFormat();
		text = flash.Lib.current.createTextField("scoretext",0,200,0,200,19);
		textform.size = 16;
		text.setNewTextFormat(textform);
		text._x = 200;
		text.text = "Score: " + Score;

		var gobi = flash.Lib.current.attachMovie("gobi", "gobi1", 2);
		gobi._x = 300;
		gobi._y = 53;

		var gobihit = flash.Lib.current.attachMovie("gobihit", "gh1",1);
		gobihit._visible = false;
		var shoe = flash.Lib.current.attachMovie("shoe", 's1',3);

		flash.Lib.current.onEnterFrame = function() {
			if (Hit == false) {
				if (gobi._x < 1) {
					GobiDir = 'right';
				}
				if (gobi._x > 418) { //418 = stage width -GobiBMP width
					GobiDir = 'left';
				}
				if (GobiDir == 'right') {
					gobi._x += GobiSpeed;
				}
				if (GobiDir == 'left') {
					gobi._x -= GobiSpeed;
				}
			}
			else {
				gobi._visible = false;
				shoe._visible = false;
				HitTimer += 1;
				if (HitTimer >= 120) {
					HitTimer = 0;
					gobihit._visible = false;
					gobi._visible = true;
//					shoe._visible = true;
					Hit = false;
				}
			}
		}

		flash.Lib.current.onMouseMove = function() {
			shoe._visible = true;
			shoe._x = flash.Lib.current._xmouse - 40;
			shoe._y = flash.Lib.current._ymouse - 20;
		}

		flash.Lib.current.onMouseDown = function() {
			if (Hit == false) {
				if (shoe.hitTest(gobi)) {
					Hit = true;
					gobihit._visible = true;
					gobihit._x = gobi._x;
					gobihit._y = 60;
					Score += 1;
				}
				else {
					Score -= 1;
				}
				text.text = "Score: " + Score;
			}
		}

		flash.Mouse.hide();

	}
}
