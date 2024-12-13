{
  "targets": [
    {
      "target_name": "keylistener",
      "sources": [ "keylistener.cpp" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}