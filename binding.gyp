{
    "targets": [{

        "target_name": "tpcall",
        "sources": [ "tpcall.cc" ],

        "include_dirs": [
            "<!(node -e \"require('nan')\")",
            "/home/gritins/projects/endurox/include"
        ],

        'libraries': [

            '-L /home/gritins/projects/endurox/dist/lib',
            '-l m',
            '-l ubf',
            '-l nstd',
            '-l atmi',
            '-l atmiclt',
            '-l rt'
        ],

        'cflags!': [ '-fno-exceptions' ],
        'cflags_cc!': [ '-fno-exceptions' ],
        'conditions': [

            ['OS=="mac"', {
                'xcode_settings': {
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
                }
            }]
        ]
    }]
}
