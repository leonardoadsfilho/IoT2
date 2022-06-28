const mqtt = require('mqtt');

const host = 'mqtt.tago.io'
const mqtt_port = '8883'
const clientId = `mqtt_${Math.random().toString(16).slice(3)}`

const connectUrl = `mqtts://${host}:${mqtt_port}`
const client = mqtt.connect(connectUrl, {
  protocol: 'mqtts',
  clientId,
  connectTimeout: 4000,
  username: 'node',
  password: 'aff30dba-aa54-4a2a-82ea-68ce0acad533',
  reconnectPeriod: 1000,
})

const topic = '98CDAC2EE8EC/peso'
client.on('connect', () => {
  console.log('Connected')
  client.subscribe([topic], () => {
    console.log(`Subscribe to topic '${topic}'`)
  })
  client.publish(topic, 'nodejs mqtt test', { qos: 0, retain: false }, (error) => {
    if (error) {
      console.error(error)
    }
  })
})

let a_meassurement = '';

client.on('message', (topic, payload) => {
  a_meassurement = payload.toString();
  console.log('Received Message:', topic, a_meassurement);
})

const port = 3001

const cors = require('cors')
const express = require('express')
const app = express()

app.use(express.json())
app.use(cors())

app.get('/', (req, res) => {
    res.send(a_meassurement)
})

app.listen(port, () => {
     console.log(`Server running at port ${port}`)
})
