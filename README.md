# android-check

[![sampctl](https://img.shields.io/badge/sampctl-android--check-2f2f2f.svg?style=for-the-badge)](https://github.com/Se8870/android-check)

A forked forked project, using y_android implementations to simplify everything.
Support`IsPlayerUsingAndroid(playerid)` just like original repo by Fairuz.

## Installation

Simply install to your project:

```bash
sampctl package install Se8870/android-check
```

Include in your code and begin using the library:

```pawn
#include <android-check>
```

## Usage

### Using callback
```pawn
#include <a_samp>
#include <android-check>

public OnClientChecked(playerid, bool:isAndroid)
{
	if (isAndroid)
	{
		return Ban(playerid);
	}
	return 1;
}
```

### Using normal functions
```pawn
#include <a_samp>
#include <android-check>

public OnPlayerSpawn(playerid)
{
	if (IsPlayerUsingAndroid(playerid))
	{
		printf("Player using android");
		return Ban(playerid);
	}
	return 1;
}
```

***NOTE:** you cannot use this inside OnPlayerConnect, that is why i made a custom one :>*

## Testing

To test, simply run the package:

```bash
sampctl run
```

## Credits

* Y_Less (rconfix hook, and y_android implementations)
* Kirima (current maintainer)
* ff-agus44 (previous maintainer)
* Fairuz-Afdhal (for the original version)
