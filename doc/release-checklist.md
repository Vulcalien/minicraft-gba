Checklist of things to do before releasing a new version.

- Make sure that the Copyright year in '/src/scene/prestart.c' is up to
  date.
- Change the version number in '/src/scene/start.c'.
- Change the 'Software Version' and 'Complement Check' bytes in
  '/src/crt0.s'; after that, make sure that the ROM is valid and that
  the GBA BIOS accepts it.
- Update the changelog file.
