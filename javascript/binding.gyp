{ 
  "targets": [ 
    { 
      "target_name": "binding", 
      "sources": [
        "binding.cc",
        "../src/buffer.c",
        "../src/memory.c",
        "../src/node.c",
        "../src/options.c",
        "../src/print.c",
        "../src/stack.c"
      ],
      "include_dirs": [
        "src",
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "depedendencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"]
    }
  ]
}
