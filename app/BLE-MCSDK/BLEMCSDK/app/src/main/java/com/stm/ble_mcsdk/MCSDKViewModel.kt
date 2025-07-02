package com.stm.ble_mcsdk

import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.stm.ble_mcsdk.activity.main.MainInterface
import com.stm.ble_mcsdk.ble.BLEManager
import com.stm.ble_mcsdk.ble.BLEResult
import com.stm.ble_mcsdk.ble.BLETimeoutException
import com.stm.ble_mcsdk.extension.intToHex
import com.stm.ble_mcsdk.extension.hexToByteArray
import com.stm.ble_mcsdk.log.LogManager
import kotlinx.coroutines.launch
import timber.log.Timber
import java.util.*

class MCSDKViewModel: ViewModel() {

    lateinit var mainInterface: MainInterface

    private var timer: Timer? = null
    private var isTimerRunning = false

    private var maxCommand = false
    val DMXval1 = MutableLiveData<Int>(0)
    val DMXch1 = MutableLiveData<Int>(1)
    val DMXval2 = MutableLiveData<Int>(0)
    val DMXch2 = MutableLiveData<Int>(2)
    val DMXval3 = MutableLiveData<Int>(0)
    val DMXch3 = MutableLiveData<Int>(3)
    val DMXval4 = MutableLiveData<Int>(0)
    val DMXch4 = MutableLiveData<Int>(4)
    val DMXval5 = MutableLiveData<Int>(0)
    val DMXch5 = MutableLiveData<Int>(5)

    var isDMXOn = false

    // Write Set Value Message from Slider
    fun setValueMessage(value: Int, channel: Int) {
        val command = "01"
        val size = "01"
        val valueHex = (value and 0xFF).intToHex(1)
        val channelHex = (channel and 0xFFFF).intToHex(2)
        val hexMessage = command + size + valueHex + channelHex

        BLEManager.scope.launch {
            writeCharacteristic(hexMessage)
            LogManager.addLog("SET Value", "Val: ($value), Ch: ($channel)")
        }
    }

    /** Write Message */

    suspend fun writeCharacteristic(message: String, max: Boolean): BLEResult? {
        // Write is a Get Max Command
        if (max) maxCommand = true

        // P2P Write Characteristic
//        val characteristic = BLEManager.getCharacteristic(
//            "B74F0000-8D4A-4C78-B601-7B2BA11022E3",
//            "B74F0002-8D4A-4C78-B601-7B2BA11022E3"
//        )
        val byteMessage = message.hexToByteArray()

        if (BLEManager.targetWriteCharacteristic != null) {
            try {
                return BLEManager.writeCharacteristic(BLEManager.targetWriteCharacteristic, byteMessage)
            } catch (e: BLETimeoutException) {
                Timber.e("BLE Timeout Error - Write")
            }
        }
        return null
    }

    suspend fun writeCharacteristic(message: String): BLEResult? {
        return writeCharacteristic(message, false)
    }

}