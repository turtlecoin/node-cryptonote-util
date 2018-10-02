{
  "targets": [
    {
      "target_name": "cryptonote",
      "sources": [
        "src/main.cc",
        "src/cryptonote_core/cryptonote_format_utils.cpp",
        "src/crypto/tree-hash.c",
        "src/crypto/crypto.cpp",
        "src/crypto/crypto-ops.c",
        "src/crypto/crypto-ops-data.c",
        "src/crypto/hash.c",
        "src/crypto/random.c",
        "src/crypto/keccak.c",
        "src/common/base58.cpp"
      ],
      "include_dirs": [
        "src",
        "src/contrib/epee/include",
        "src/contrib/variant/include",
        "<!(node -e \"require('nan')\")"
      ],
      "cflags!": [
        "-fno-exceptions"
      ],
      "cflags_cc!": [
        "-fno-exceptions",
        "-fno-rtti"
      ],
      "cflags_cc": [
        "-std=c++0x",
        "-fexceptions",
        "-frtti"
      ],
      "conditions": [
        [
          "OS!='win'",
          {
            "link_settings": {
              "libraries": [
                "-lboost_system",
                "-lboost_date_time"
              ]
            }
          },
          "OS=='win'",
          {
            "include_dirs": [
              "<(module_root_dir)/winboost",
              "src/platform/msc"
            ],
            "link_settings": {
              "libraries": [
                "<(module_root_dir)/winboost/libs/*.lib"
              ]
            },
            "configurations": {
              "Release": {
                "msvs_settings": {
                  "VCCLCompilerTool": {
                    "RuntimeLibrary": 0,
                    "Optimization": 3,
                    "FavorSizeOrSpeed": 1,
                    "InlineFunctionExpansion": 2,
                    "WholeProgramOptimization": "true",
                    "OmitFramePointers": "true",
                    "EnableFunctionLevelLinking": "true",
                    "EnableIntrinsicFunctions": "true",
                    "RuntimeTypeInfo": "false",
                    "ExceptionHandling": "0",
                    "AdditionalOptions": [
                      "/MP /EHsc -D_WIN32_WINNT=0x0501"
                    ]
                  },
                  "VCLibrarianTool": {
                    "AdditionalOptions": [
                      "/LTCG"
                    ]
                  },
                  "VCLinkerTool": {
                    "LinkTimeCodeGeneration": 1,
                    "OptimizeReferences": 2,
                    "EnableCOMDATFolding": 2,
                    "LinkIncremental": 1,
                    "AdditionalLibraryDirectories": [
                    ]
                  }
                }
              }
            }
          }
        ]
      ]
    }
  ]
}