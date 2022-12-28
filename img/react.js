import { useCallback, useEffect, useState } from 'react';
import { GET_ALL_SPECTRUMS } from '../../constants/endpoints';
import plots from './index'


const base64ToArrayBuffer = base64 => {
    let binary_string = window.atob(base64);
    let len = binary_string.length;
    let bytes = new Uint8Array(len);
    for (let i = 0; i < len; i++) {
        bytes[i] = binary_string.charCodeAt(i);
    }
    return bytes.buffer;
}

const Spectrum = () => {

    const [loading, setLoading] = useState(true);
    const [module, setModule] = useState();

    let SSE = new EventSource(GET_ALL_SPECTRUMS)

    SSE.onmessage = useCallback(msg => {
        let buffer = base64ToArrayBuffer(msg.data)
        let view = new DataView(buffer)

        let spectrums_count = view.getUint32(0,true) - 1; // 12 plots
        let reports_count = view.getUint32(4, true);
        let spectrum_config_size = 4 + 4 + (4 * reports_count);
        let dots_list = [];

        // console.log(spectrums_count)
        // console.log(reports_count)
        // console.log(spectrum_config_size)
        if(!loading) {
            const logger = module.cwrap('logger', 'string', null);
            module.ccall('set_fps', null, ['number'], [100])

            for(let spec_idx = 0; spec_idx < spectrums_count; spec_idx++) {
                let offset = 8 + (spec_idx * spectrum_config_size);
                let dots = new Float32Array(buffer.slice(offset + 8, offset + spectrum_config_size));
                var dots_buffer = module._malloc( dots.length * dots.BYTES_PER_ELEMENT);
                module.HEAPF32.set(dots, dots_buffer >> 2);
                dots_list.push(dots_buffer);
                module._push_data(dots_buffer, dots.length, 1);
            }
            module._draw_plots_data();
            dots_list.map((dbuf) => module._free(dbuf));
        }
    }, [module, loading])

    useEffect(() => {
        plots({ canvas: document.querySelector("#canvas") }).then((module) => {
            setModule(module);
            setLoading(false);
        });

    }, []);

    return (
        <div>
            {!loading ? console.log("Done loading") : "Loading..."}
            <canvas id="canvas" />
        </div>
    )
}

export default Spectrum;

// import { GET_ALL_SPECTRUMS } from '../../constants/endpoints'

// let spectrums_socket = null

// const base64ToArrayBuffer = base64 => {
//     let binary_string = window.atob(base64);
//     let len = binary_string.length;
//     let bytes = new Uint8Array(len);
//     for (let i = 0; i < len; i++) {
//         bytes[i] = binary_string.charCodeAt(i);
//     }
//     return bytes.buffer;
// }

// export const getSpectrums = () => {
//     return dispatch => {
//         spectrums_socket = new EventSource(GET_ALL_SPECTRUMS)

//         spectrums_socket.onmessage = msg => {
//             let buffer = base64ToArrayBuffer(msg.data)
//             let view = new DataView(buffer)

//             let spectrums_count = view.getUint32(0, true)
//             let reports_count = view.getUint32(4, true)
//             let spectrum_config_size = 4 + 4 + (4 * reports_count)

//             let spectrums = []

//             for(let spec_idx = 0; spec_idx < spectrums_count; spec_idx++) {
//                 let offset = 8 + (spec_idx * spectrum_config_size)
//                 let spec = {
//                     dx: view.getFloat32(offset, true),
//                     x0: view.getFloat32(offset + 4, true),
//                     fft_reports: new Float32Array(buffer.slice(offset + 8, offset + spectrum_config_size))
//                 }

//                 let trace = {
//                     x0: spec.x0,
//                     dx: spec.dx,
//                     y: spec.fft_reports,
//                     // line: {color: 'royalblue', simplify: false},
//                     // mode: 'lines',
//                     // type: 'scatter',
//                     // showlegend: false
//                 }
//                 spectrums.push(trace)
//             }
//             dispatch(saveSpectrums(spectrums))
//         }

// }}

// export const saveSpectrums = spectrums => ({
//     type: 'SAVE_SPECTRUMS',
//     spectrums: spectrums
// })
